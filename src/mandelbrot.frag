#version 330 core

/*
 * z(n-1) = z(n)**2 + c
 * z0 = 0
 * z1 = z0 ** 2 + c -> c            1st iteration
 * z2 = z1 ** 2 + c -> c**2 + c     2nd iteration
*/

uniform vec3 colormap[256];
uniform vec2 resolution;
uniform float bound;
uniform int iters;
uniform float colormapStep;

float map(float value, float min1, float max1, float min2, float max2) {
  //return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
  return min2 + value * (max2 - min2);
}

void main() {
  vec2 uv = gl_FragCoord.xy / resolution;
  vec3 col = vec3(1.f);

  float a = map(uv.x, 0.f, 1.f, -bound, bound);
  float b = map(uv.y, 0.f, 1.f, -bound, bound);

  // The default "c" component
  float ca = a;
  float cb = b;

  int n = 0;
  for (; abs(a + b) < 200 && n < iters; n++) {
    // This two make the "z(n-1)²" component
    float aa = a * a - b * b;
    float bb = 2.f * a * b;

    // This two make the "z(n)²" component
    a = aa + ca;
    b = bb + cb;
  }

  //col = colormap[int(n * colormapStep)];
  float alpha = float(n) / iters;
  alpha = n == iters ? 0.f : sqrt(alpha);

  gl_FragColor = vec4(col, alpha);
}

