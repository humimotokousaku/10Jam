#include "Object3d.hlsli"

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
	float32_t shininess;
};
// 平行光源
struct DirectionalLight {
	float32_t4 color;
	float32_t3 direction;
	float intensity;
};
// 点光源
struct PointLight {
	float32_t4 color;
	float32_t3 pos;
	float intensity;
	float radius;
	float decay;
};
// スポットライト
struct SpotLight {
	float32_t4 color;
	float32_t3 pos;
	float intensity;
	float32_t3 direction;
	float distance;
	float decay;
	float cosAngle;
	float cosFalloffStart;
};
// カメラの位置
struct Camera {
	float32_t3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<PointLight> gPointLight : register(b3);
ConstantBuffer<SpotLight> gSpotLight : register(b4);
ConstantBuffer<Camera> gCamera : register(b2);

Texture2D<float32_t4> gTexture : register(t0);
TextureCube<float32_t4> gEnvironmentTexture : register(t1);
SamplerState gSampler : register(s0);
struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	// textureのα値が0.0以下の時にPixelを棄却
	if (textureColor.a <= 0.0) {
		discard;
	}
			
	// 環境マップ
    float32_t3 cameraToPosition = normalize(input.worldPosition - gCamera.worldPosition);
    float32_t3 reflectedVector = reflect(cameraToPosition, normalize(input.normal));
    float32_t4 enviromentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);
		
	// ライティングをする場合
	if (gMaterial.enableLighting != 0) {
		// ハーフランバート
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NdotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NdotH), gMaterial.shininess);

		// 拡散反射
        float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		// 鏡面反射
        float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
		// 拡散反射 + 鏡面反射
        output.color.rgb = diffuse;// + specular;
		// アルファ値
        output.color.a = gMaterial.color.a * textureColor.a;
	}
	else {
        output.color = gMaterial.color * textureColor;
    }

	return output;
}
