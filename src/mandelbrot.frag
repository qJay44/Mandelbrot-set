#version 330 core

/*
 * z(n-1) = z(n) * z(n) + c
 * z0 = 0
 * z1 = z0 * z0 + c -> c            1st iteration
 * z2 = z1 * z1 + c -> c * c + c    2nd iteration
*/

uniform vec3 colormap[256];
uniform vec2 resolution;
uniform vec2 boundsX;
uniform vec2 boundsY;

uniform vec2 move;
uniform float zoom;
uniform int iters;

void main() {
  vec2 uv = gl_FragCoord.xy / resolution;
  vec3 col = vec3(1.f);

  // Scaled to lie in the Mandelbrot scale
  float x0 = boundsX.x + uv.x * (boundsX.y - boundsX.x);
  float y0 = boundsY.x + uv.y * (boundsY.y - boundsY.x);

  x0 = x0 / zoom + move.x;
  y0 = y0 / zoom + move.y;

  float x = 0.f;
  float y = 0.f;
  int n = 0;

  while (x * x + y * y <= 4.f && n++ < iters) {
    float xTemp = x * x - y * y + x0;
    y = 2.f * x * y + y0;
    x = xTemp;
  }

  float smoothed = log2(log2(x * x + y * y) * 0.5f);
  int index = int(sqrt(n + 10.f - smoothed) * 8);
  col = colormap[index % 256];

  gl_FragColor = vec4(col, 1.f);
}

