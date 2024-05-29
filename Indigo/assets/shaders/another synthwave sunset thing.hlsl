#include "assets/shaders/ShaderCommon.hlsli"

#define vec2 float2
#define vec3 float3
#define vec4 float4
#define mod fmod
#define mix lerp

#define AA 2
#define speed 10.0
#define wave_thing

//self-explanatory
#define audio_vibration_amplitude 0.125

float jTime;

#define textureMirror(a, b) float4(0, 0, 0, 0)

float amp(float2 p)
{
    return smoothstep( 1.0, 8.0, abs( p.x ) );
}

float pow512( float a )
{
    a *= a; // ^2
    a *= a; // ^4
    a *= a; // ^8
    a *= a; // ^16
    a *= a; // ^32
    a *= a; // ^64
    a *= a; // ^128
    a *= a; // ^256
    return a * a;
}

float pow1d5( float a )
{
    return a * sqrt( a );
}

float hash21( float2 co )
{
    return frac( sin( dot( co, float2( 1.9898, 7.233 ) ) ) * 45758.5433 );
}

float hash( float2 uv )
{
    float a = amp(uv);
#ifdef wave_thing
    float w = a > 0.0 ? (1.0 - 0.4 * pow512( 0.51 + 0.49 * sin( (0.02 * (uv.y + 0.5 * uv.x) - jTime) * 2.0 ) )) : 0.0;
#else
    float w = 1.0;
#endif
    return (a > 0.0 ? a * pow1d5( hash21( uv ) ) * w : 0.0) - (textureMirror( 0, float2( (uv.x * 29.0 + uv.y) * 0.03125, 1.0 ) ).x) * audio_vibration_amplitude;
}

float edgeMin( float dx, float2 da, float2 db, float2 uv )
{
    uv.x += 5.0;
    float3 c = frac( (round( float3( uv, uv.x + uv.y ) )) * (float3( 0.0, 1.0, 2.0 ) + 0.61803398875) );
    float a1 = textureMirror( 0, float2( c.y, 0.0 ) ).x > 0.6 ? 0.15 : 1.0;
    float a2 = textureMirror( 0, float2( c.x, 0.0 ) ).x > 0.6 ? 0.15 : 1.0;
    float a3 = textureMirror( 0, float2( c.z, 0.0 ) ).x > 0.6 ? 0.15 : 1.0;

    return min( min( (1.0 - dx) * db.y * a3, da.x * a2 ), da.y * a1 );
}

float2 trinoise( float2 uv )
{
    const float sq = sqrt( 3.0 / 2.0 );
    uv.x *= sq;
    uv.y -= 0.5 * uv.x;
    float2 d = frac( uv );
    uv -= d;

    bool c = dot( d, float2( 1.0 ) ) > 1.0;

    float2 dd = 1.0 - d;
    float2 da = c ? dd : d, db = c ? d : dd;

    float nn = hash( uv + float( c ) );
    float n2 = hash( uv + float2( 1.0, 0.0 ) );
    float n3 = hash( uv + float2( 0.0, 1.0 ) );


    float nmid = lerp( n2, n3, d.y );
    float ns = lerp( nn, c ? n2 : n3, da.y );
    float dx = da.x / db.y;
    return float2( lerp( ns, nmid, dx ), edgeMin( dx, da, db, uv + d ) );
}

float2 map( float3 p )
{
    float2 n = trinoise( p.xz );
    return float2( p.y - 2.0 * n.x, n.y );
}

float3 grad( float3 p )
{
    const float2 e = float2( 0.005, 0.0 );
    float a = map( p ).x;
    return float3( map( p + e.xyy ).x - a,
        map( p + e.yxy ).x - a,
        map( p + e.yyx ).x - a ) / e.x;
}

float2 intersect( float3 ro, float3 rd )
{
    float d = 0.0, h = 0.0;
    for (int i = 0; i < 500; i++)
    { //look nice with 50 iterations
        float3 p = ro + d * rd;
        float2 s = map( p );
        h = s.x;
        d += h * 0.5;
        if (abs( h ) < 0.003 * d)
            return float2( d, s.y );
        if (d > 150.0 || p.y > 2.0) break;
    }

    return float2( -1.0 );
}

void addsun( float3 rd, float3 ld, inout float3 col )
{
    float sun = smoothstep( 0.21, 0.2, distance( rd, ld ) );

    if (sun > 0.0)
    {
        float yd = (rd.y - ld.y);

        float a = sin( 3.1 * exp( -(yd) * 14.0 ) );

        sun *= smoothstep( -0.8, 0.0, a );

        col = lerp( col, float3( 1.0, 0.8, 0.4 ) * 0.75, sun );
    }
}

float starnoise( float3 rd )
{
    float c = 0.0;
    float3 p = normalize( rd ) * 300.0;
    for (float i = 0.0; i < 4.0; i++)
    {
        float3 q = frac( p ) - 0.5;
        float3 id = floor( p );
        float c2 = smoothstep( 0.5, 0.0, length( q ) );
        c2 *= step( hash21( id.xz / id.y ), 0.06 - i * i * 0.005 );
        c += c2;
        p = p * 0.6 + 0.5 * p * float3x3( 3.0 / 5.0, 0, 4.0 / 5.0, 0, 1, 0, -4.0 / 5.0, 0, 3.0 / 5.0 );
    }
    c *= c;
    float g = dot( sin( rd * 10.512 ), cos( rd.yzx * 10.512 ) );
    c *= smoothstep( -3.14, -0.9, g ) * 0.5 + 0.5 * smoothstep( -0.3, 1.0, g );
    return c * c;
}

float3 gsky( float3 rd, float3 ld, bool mask )
{
    float haze = exp2( -5.0 * (abs( rd.y ) - 0.2 * dot( rd, ld )) );

    float st = mask ? (starnoise( rd )) * (1.0 - min( haze, 1.0 )) : 0.0;
    float3 back = float3( 0.4, 0.1, 0.7 ) * (1.0 - 0.5 * textureMirror( 0, float2( 0.5 + 0.05 * rd.x / rd.y, 0.0 ) ).x
        * exp2( -0.1 * abs( length( rd.xz ) / rd.y ) )
        * max( sign( rd.y ), 0.0 ));
    float3 col = clamp( lerp( back, float3( 0.7, 0.1, 0.4 ), haze ) + st, 0.0, 1.0 );
    if (mask) addsun( rd, ld, col );
    return col;
}

float4 PSmain( VSOut i ) : SV_TARGET{
    float2 fragCoord = i.uv;
    float2 iResolution = float2( 1600, 900 );
    float2 uv = i.uv;
    uv = uv * 2.0 - 1.0;
    uv.x *= 16.0 / 9.0;
    float battery = 1.0;
    float4 fragColor;

    const float AA = 1.0, x = 0.0, y = 0.0;

    const float shutter_speed = 0.25; // for motion blur
    float dt = frac( hash21( float( AA ) * (fragCoord + float2( x, y )) ) + Time ) * shutter_speed;
    jTime = fmod( Time - dt * Time, 4000.0 );
    float3 ro = float3( 0.0, 1.0, (-20000.0 + jTime * speed) );
    float3 rd = normalize( float3( uv, 4.0 / 3.0 ) );

    float2 i = intersect( ro, rd );
    float d = i.x;

    float3 ld = normalize( float3( 0.0, 0.125 + 0.05 * sin( 0.1 * jTime ), 1.0 ) );
    float3 fog = d > 0.0 ? exp2( -d * float3( 0.14, 0.1, 0.28 ) ) : float3( 0.0 );
    float3 sky = gsky( rd, ld, d < 0.0 );

    float3 p = ro + d * rd;
    float3 n = normalize( grad( p ) );

    float diff = dot( n, ld ) + 0.1 * n.y;
    float3 col = float3( 0.1, 0.11, 0.18 ) * diff;

    float3 rfd = reflect( rd, n );
    float3 rfcol = gsky( rfd, ld, true );

    col = lerp( col, rfcol, 0.05 + 0.95 * pow( max( 1.0 + dot( rd, n ), 0.0 ), 5.0 ) );
    col = lerp( col, float3( 0.8, 0.1, 0.92 ), smoothstep( 0.05, 0.0, i.y ) );
    col = lerp( sky, col, fog );

    if (d < 0.0)
        d = 1e6;
    d = min( d, 10.0 );
    fragColor = float4( clamp( col, 0.0, 1.0 ), d < 0.0 ? 0.0 : 0.1 + exp2( -d ) );
    return fragColor;
}
