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
    // Z値を出力する
    //return LinearizeDepth(input.Position.z,0.1f,100.0f);
    //return input.Position.z;
    return float4(input.Position.z, input.Position.z * input.Position.z, 0, 0);
}
