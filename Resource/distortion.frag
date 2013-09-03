uniform vec2 scale;
uniform vec2 scaleIn;
uniform vec2 lensCenter;
uniform vec4 hmdWarpParam;
uniform sampler2D texid;
void main()
{
  vec2 uv = (gl_TexCoord[0].st * 2.0) - 1.0; // range from [0,1] to [-1,1]
  vec2 theta = (uv-lensCenter) * scaleIn;
  float rSq = theta.x * theta.x + theta.y * theta.y;
  vec2 rvector = theta * (
    hmdWarpParam.x +
    hmdWarpParam.y * rSq +
    hmdWarpParam.z * rSq * rSq +
    hmdWarpParam.w * rSq * rSq * rSq
  );
  vec2 tc = (lensCenter + scale * rvector);
  tc = (tc + 1.0) / 2.0; // range from [-1,1] to [0,1]
  if (any(bvec2(clamp(tc, vec2(0.0,0.0), vec2(1.0,1.0)) - tc)))
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  else
    gl_FragColor = texture2D(texid, tc);
}
