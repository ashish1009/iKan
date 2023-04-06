//
//  pbr_static_shader.glsl
//  ikan
//
//  Created by iKan on 24/11/22.
//

// vertex Shader
#type vertex

#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Binormal;
layout (location = 5) in int  a_ObjectID;

out VS_OUT
{
  vec3 WorldPosition;
  vec3 Normal;
  vec2 TexCoord;
  mat3 WorldNormals;
  mat3 WorldTransform;
  vec3 Binormal;
  float ObjectID;
} vs_Output;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

// Texture Invert
uniform float u_TextureInvertVertical;
uniform float u_TextureInvertHorizontal;

/// Vertex Main Function
void main()
{
  vs_Output.WorldPosition = vec3(u_Transform * vec4(a_Position, 1.0));
  vs_Output.Normal = mat3(u_Transform) * a_Normal;
  vs_Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
  vs_Output.WorldNormals = mat3(u_Transform) * mat3(a_Tangent, a_Binormal, a_Normal);
  vs_Output.WorldTransform = mat3(u_Transform);
  vs_Output.Binormal = a_Binormal;
  vs_Output.ObjectID  = a_ObjectID;
  
  float normalizedTexCoordsY = (u_TextureInvertVertical > 0.5) ? 1.0f - vs_Output.TexCoord.y : vs_Output.TexCoord.y;
  float normalizedTexCoordsX = (u_TextureInvertHorizontal > 0.5) ? 1.0f - vs_Output.TexCoord.x : vs_Output.TexCoord.x;
  vec2 normalizedTexCoords = vec2(normalizedTexCoordsX, normalizedTexCoordsY);

  vs_Output.TexCoord = normalizedTexCoords;
  
  gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

// Fragment Shader
#type fragment

#version 330 core
layout (location = 0) out vec4 o_Color;
layout (location = 1) out vec4 o_BrightColor;
layout (location = 2) out int  o_IDBuffer;

const float PI = 3.14159265359;
const float Epsilon = 0.00001;

in VS_OUT
{
  vec3 WorldPosition;
  vec3 Normal;
  vec2 TexCoord;
  mat3 WorldNormals;
  mat3 WorldTransform;
  vec3 Binormal;
  float ObjectID;
} vs_Input;

// Stores the Material Property
struct Material
{
  vec3 AlbedoColor;
  
  float Metalness;
  float Roughness;
  float TilinghFactor;
};
uniform Material u_Material;

// Stores the Light Property
struct Light
{
  bool Present;
  bool Attenuation;
  
  vec3 Position;
  vec3 Radiance;
  
  float Multiplier;
};
// TODO: Add this 4 as variable from outside
uniform Light u_PointLights[4];

// Texture Uniforms
uniform sampler2D u_AlbedoTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_RoughnessTexture;
uniform sampler2D u_MetallicTexture;

// IBL
uniform samplerCube u_IrradianceMap;
uniform samplerCube u_EnvRadianceTex;
uniform sampler2D u_BrdfTexture;

// Shadow depth map
uniform samplerCube u_DepthMap;

// Texture Toggle
uniform float u_AlbedoTextureToggle;
uniform float u_NormalTextureToggle;
uniform float u_RoughnessTextureToggle;
uniform float u_MetallicTextureToggle;

uniform float u_IblFlag;
uniform float u_IsShadow;

// Camera Params
uniform vec3 u_CameraPosition;
uniform float u_IsGamma;
uniform float u_IsHdr;
uniform float u_Exposure;
uniform float u_Gamma;

struct PBRParameters
{
  vec3 Albedo;
  vec3 Normal;
  
  vec3 View;
  float NdotV;
  
  float Metalness;
  float Roughness;
};
PBRParameters m_Params;

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2
float ndfGGX(float cosLh, float roughness)
{
  float alpha = roughness * roughness;
  float alphaSq = alpha * alpha;
  
  float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
  return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
  return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float NdotV, float roughness)
{
  float r = roughness + 1.0;
  float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
  return gaSchlickG1(cosLi, k) * gaSchlickG1(NdotV, k);
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
  return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Slick Roughness for IBL
vec3 fresnelSchlickRoughness(vec3 F0, float cosTheta, float roughness)
{
  return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

/// Add Light to Material
vec3 PointLighting(vec3 F0)
{
  vec3 result = vec3(0.0);
  
  // Process each light
  for (int i = 0; i < 4; i++)
  {
    Light light = u_PointLights[i];
    if (light.Present)
    {
      // Light Direction
      vec3 Li = normalize(light.Position - vs_Input.WorldPosition);
      // half way vector
      vec3 Lh = normalize(Li + m_Params.View);
      
      float attenuation = 1.0f;
      if (light.Attenuation)
      {
        float distance = length(light.Position - vs_Input.WorldPosition);
        attenuation = (1.0 / (distance * distance)) * light.Multiplier;
      }
      
      // Light Radiance
      vec3 Lradiance = light.Radiance * attenuation;
      
      // Calculate angles between surface normal and various light vectors.
      float cosLi = max(0.0, dot(m_Params.Normal, Li));
      float cosLh = max(0.0, dot(m_Params.Normal, Lh));
      
      // Cook-Torrance BRDF
      float D = ndfGGX(cosLh, m_Params.Roughness);
      float G = gaSchlickGGX(cosLi, m_Params.NdotV, m_Params.Roughness);
      vec3 F = fresnelSchlick(F0, max(0.0, dot(Lh, m_Params.View)));
      
      // BRDF Specular
      vec3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * m_Params.NdotV);
      
      // BRDF Diffuse
      // for energy conservation, the diffuse and specular light can't
      // be above 1.0 (unless the surface emits light); to preserve this
      // relationship the diffuse component (kD) should equal 1.0 - F.
      
      // multiply kD by the inverse metalness such that only non-metals
      // have diffuse lighting, or a linear blend if partly metal (pure metals
      // have no diffuse light).
      vec3 kd = (1.0 - F) * (1.0 - m_Params.Metalness);
      vec3 diffuseBRDF = kd * m_Params.Albedo;
      
      result += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
    }
  }
  
  return result;
}

/// Calculate the Ambient factor of Scene
vec3 IBL(vec3 F0)
{
  vec3 F = fresnelSchlickRoughness(F0, m_Params.NdotV, m_Params.Roughness);
  vec3 kS = F;
  vec3 kD = 1.0 - kS;
  
  vec3 irradiance = texture(u_IrradianceMap, m_Params.Normal).rgb;
  vec3 diffuse    = irradiance * m_Params.Albedo;
  
  // sample both the pre-filter map and the BRDF lut and combine them together
  // as per the Split-Sum approximation to get the IBL specular part.
  const float MAX_REFLECTION_LOD = 4.0;
  
  vec3 R = reflect(-m_Params.View, m_Params.Normal);
  vec2 brdf = texture(u_BrdfTexture, vec2(max(m_Params.NdotV, 0.0), m_Params.Roughness)).rg;
  vec3 prefilteredColor = textureLod(u_EnvRadianceTex, R,  m_Params.Roughness * MAX_REFLECTION_LOD).rgb;
  
  vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
  vec3 ambient = (kD * diffuse + specular);
  
  return ambient;
}

/// Shadow calculation
float ShadowCalculation(vec3 fragPos)
{
  float shadow = 0.0f;
  if (u_IsShadow > 0.5f)
  {
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - u_PointLights[0].Position;
    
    // ise the fragment to light vector to sample from the depth map
    float closestDepth = texture(u_DepthMap, fragToLight).r;
    
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= 10000.0f;
    
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    
    // test for shadows
    float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, 10000.0f] range
    shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
  }
  return shadow;
}

/// Fragment Main Function
void main()
{
  // TODO: Make things in vertex Shader if possible
  // Setup Material property
  m_Params.Albedo    = (u_AlbedoTextureToggle > 0.5) ? texture(u_AlbedoTexture, vs_Input.TexCoord * u_Material.TilinghFactor).rgb : u_Material.AlbedoColor;
  m_Params.Metalness = (u_MetallicTextureToggle > 0.5) ? texture(u_MetallicTexture, vs_Input.TexCoord * u_Material.TilinghFactor).r : u_Material.Metalness;
  m_Params.Roughness = (u_RoughnessTextureToggle > 0.5) ? texture(u_RoughnessTexture, vs_Input.TexCoord * u_Material.TilinghFactor).r : u_Material.Roughness;
  m_Params.Roughness = max(m_Params.Roughness, 0.05); // Minimum roughness of 0.05 to keep specular highlight

  // Normals (either from vertex or map)
  m_Params.Normal = normalize(vs_Input.Normal);
  if (u_NormalTextureToggle > 0.5)
  {
    m_Params.Normal = 2.0 * texture(u_NormalTexture, vs_Input.TexCoord * u_Material.TilinghFactor).rgb - 1.0f;
    m_Params.Normal = normalize(vs_Input.WorldNormals * m_Params.Normal);
  }
  
  m_Params.View = normalize(u_CameraPosition - vs_Input.WorldPosition);
  m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0f);

  // calculate reflectance at normal incidence
  // By pre-computing F0 for both dielectrics and conductors we can use the same Fresnel-Schlick approximation for both types of surfaces,
  // but we do have to tint the base reflectivity if we have a metallic surface. We generally accomplish this as follows:
  vec3 F0 = vec3(0.04f);
  F0 = mix(F0, m_Params.Albedo, m_Params.Metalness);

  vec3 lightContribution = PointLighting(F0);
  vec3 iblContribution;
  if (u_IblFlag > 0.5)
  {
    iblContribution = IBL(F0);
  }
  else
  {
    iblContribution = vec3(0.0f);
  }

  float shadowContribution = ShadowCalculation(vs_Input.WorldPosition);

  vec3 color = lightContribution + iblContribution;
  color *= (1.0 - shadowContribution);

  if (u_IsHdr > 0.5f)
    color = vec3(1.0) - exp(-color * u_Exposure);
  
  if(u_IsGamma > 0.5f) {
    color = pow(color, vec3(1.0/u_Gamma));
  }

  o_Color = vec4(color, 1.0f);
  o_IDBuffer = int(vs_Input.ObjectID);
  
  float brightness = dot(o_Color.rgb, vec3(0.2126, 0.7152, 0.0722));
  if(brightness > 0.8)
    o_BrightColor = vec4(o_Color.rgb, 1.0);
  else
    o_BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
