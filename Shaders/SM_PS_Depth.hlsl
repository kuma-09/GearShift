struct PS_INPUT
{
    float4 Position : SV_Position;
};

float4 main(PS_INPUT input) : SV_TARGET0
{
    // Z�l���o�͂���
    return input.Position.z;
}
