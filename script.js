const canvas = document.querySelector('canvas');
const ctx = canvas.getContext('2d');

const w = canvas.width = innerHeight;
const h = canvas.height = innerHeight;

const imgData = ctx.createImageData(w, h);
const data = imgData.data;
for (let x = 0; x < w; x++) {
    for (let y = 0; y < w; y++) {
        let a = normalize(x, [0, w], [-2, 2]);
        let b = normalize(y, [0, h], [-2, 2]);

        let z = 0;
        let n = 0;

        for (n; n < 100; n++) {
            let aa = a ** 2 - b ** 2;
            let bb = 2 * a * b;

            a = aa;
            b = bb;

            if (Math.abs(a + b) > 16) break;
        }

        let bright = 0;
        if (n == 100) {
            bright = 255;
        }

        let pix = (x + y * w) * 4;
        data[pix + 0] = bright;
        data[pix + 1] = bright;
        data[pix + 2] = bright;
        data[pix + 3] = 255;
    }
}

function normalize(val, currentRange, targetRange) {
    const currStart = currentRange[0]
    const currEnd = currentRange[1];
    const tarStart = targetRange[0];
    const tarEnd = targetRange[1];

    const currLength = currEnd - currStart;
    const tarLength = tarEnd - tarStart;

    const currStepNum = val - currStart;
    const tarStepNum = (tarLength * currStepNum) / currLength;

    return tarStart + tarStepNum;
}

ctx.putImageData(imgData, 0, 0);

