#version 330 core

/*
 * z(n-1) = z(n) * z(n) + c
 * z0 = 0
 * z1 = z0 * z0 + c -> c            1st iteration
 * z2 = z1 * z1 + c -> c * c + c     2nd iteration
*/

uniform vec3 colormap[256];
uniform vec2 resolution;
uniform vec2 boundsX;
uniform vec2 boundsY;
uniform int iters;

float map(float value, float min1, float max1, float min2, float max2) {
  //return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
  return min2 + value * (max2 - min2);
}

void main() {
  vec2 uv = gl_FragCoord.xy / resolution;
  vec3 col = vec3(1.f);

  // Scaled to lie in the Mandelbrot scale
  float x0 = map(uv.x, 0.f, 1.f, boundsX.x, boundsX.y);
  float y0 = map(uv.y, 0.f, 1.f, boundsY.x, boundsY.y);

  float x = 0.f; // The real component
  float y = 0.f; // The imaginary component
  int n = 0;

  while (x * x + y * y <= (1 << 16) && n++ < iters) {
    float xTemp = x * x - y * y + x0;
    y = 2.f * x * y + y0;
    x = xTemp;
  }

  float smoothed = log2(log2(x * x + y * y) * 0.5f);
  int index = int(sqrt(n + 10.f - smoothed) * 8) % 256;
  col = colormap[index];

  gl_FragColor = vec4(col, 1.f);
}

