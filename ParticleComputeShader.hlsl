
struct Particle
{
    float3 position;
    float3 size;
    float3 velocity;
    float lifetime;
};

RWStructuredBuffer<Particle> particles;
float particleCount;

// ハッシュベースの擬似乱数関数
float Hash(float seed)
{
    seed = frac(sin(seed) * 43758.5453123);
    return seed;
}

// IDに基づく擬似乱数生成
float Random(uint id, float offset)
{
    return Hash(float(id) + offset);
}

[numthreads(256,1,1)]
void main( uint3 dispatchID : SV_DispatchThreadID )
{
    uint id = dispatchID.x;
    
    //重力の適用
    particles[id].lifetime -= 1.0f;
    particles[id].size.x = max(particles[id].size.x - 0.005f, 0.1f);
    particles[id].size.y = max(particles[id].size.y - 0.005, 0.1); // 高さの最小値
    particles[id].size.z = max(particles[id].size.z - 0.005, 0.1); // 奥行きの最小値

    // ライフタイムが終了した場合の再初期化
    if (particles[id].lifetime <= 0)
    {
        particles[id].position = float3(0, 0, 0);
        particles[id].velocity = float3(Random(id, 1.0) * 2.0f - 1.0f, Random(id, 1.0) * 2.0f - 2.0f, Random(id, 1.0) * 2.0f - 1.0f);
        particles[id].lifetime = 5.0;
        particles[id].size = float3(
            1.0f, // 幅
            1.0f, // 高さ
            1.0f // 奥行き
        );
    }
    
    
}