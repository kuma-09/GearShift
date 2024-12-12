struct PS_INPUT
{
    float4 Position : SV_Position;
};

float LinearizeDepth(float depth, float near, float far)
{
    return (2.0 * near) / (far + near - depth * (far - near));
}

float4 main(PS_INPUT input) : SV_TARGET
{
    // Z’l‚ğo—Í‚·‚é
    return LinearizeDepth(input.Position.z,0.1f,100.0f);
}
