
struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

// ���_�V�F�[�_�[
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;


    //// ���_���W�̕ϊ�
    output.Pos = float4(input.Pos, 1);

    // �e�N�X�`�����W�͂��̂܂�
    output.Tex = input.Tex;


    return output;
}
