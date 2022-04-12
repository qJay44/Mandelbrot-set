const canvas = document.querySelector('canvas');
const ctx = canvas.getContext('2d');

const w = canvas.width = innerHeight - 50;
const h = canvas.height = innerHeight - 50;

const imgData = ctx.createImageData(w, h);
const data = imgData.data;

const max_iter = 100;
const minVal = -2.5;
const maxVal = 2.5;
for (let x = 0; x < w; x++) {
    for (let y = 0; y < w; y++) {
        let a = normalize(x, 0, w, minVal, maxVal);
        let b = normalize(y, 0, h, minVal, maxVal);

        let ca = a;
        let cb = b;

        let n = 0;

        for (n; n < max_iter; n++) {
            let z = a ** 2 - b ** 2;
            let c = 2 * a * b;

            a = z + ca;
            b = c + cb;

            if (Math.abs(a + b) > 20) break;
        }

        let bright = normalize(n, 0, max_iter, 0, 1);
        bright = normalize(Math.sqrt(bright), 0, 1, 0, 255);
        if (n == max_iter) {
            bright = 0;
        }

        let pix = (x + y * w) * 4;
        data[pix + 0] = bright;
        data[pix + 1] = bright;
        data[pix + 2] = bright;
        data[pix + 3] = 255;
    }
}

function normalize(val, ...ranges) {
    const currStart     = ranges[0];
    const currEnd       = ranges[1];
    const tarStart      = ranges[2];
    const tarEnd        = ranges[3];

    const currLength = currEnd - currStart;
    const tarLength = tarEnd - tarStart;

    const currStepNum = val - currStart;
    const tarStepNum = (tarLength * currStepNum) / currLength;

    return tarStart + tarStepNum;
}

ctx.putImageData(imgData, 0, 0);
