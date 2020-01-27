#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

class Pixel {
public:
    Pixel() = default;

    Pixel(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}

    unsigned char r, g, b;

    friend istream & operator>>(istream & is, Pixel & pixel) {
        pixel.r = is.get();
        pixel.g = is.get();
        pixel.b = is.get();
        return is;
    }

    friend ostream & operator<<(ostream & os, Pixel & pixel) {
        os <<  pixel.r <<  pixel.g <<  pixel.b;
        return os;
    }
};

void print_pic(vector<Pixel> & v) {
    int c = 1;
    for (auto & pixel: v) {
        cout << setw(4) << hex << (int) pixel.r << " " << (int) pixel.g << " " << (int) pixel.b << " ";
        if (c % 300 == 0)
            cout << endl;
        c++;
    }
    cout << endl;
}

struct matrix {
    int8_t a1 = -1; // left
    int8_t a2 = 5;  // middle
} m;

void get_w_h(ifstream & file, int & w, int & h) {
    file.seekg(2, file.beg);
    file >> w >> h;
}

void open_pic(ifstream & file, vector<Pixel> & pixels, int w, int h) {
    int a;
    file >> a;
    file.get();
    pixels.resize(w * h);
    for (auto & pp : pixels) {
        file >> pp;
    }

}

int main(int ac, char ** av) {
    ifstream file(av[1], ios::binary);

    if (!file.is_open() || !file.good()) {
        exit(1);
    }

    vector<Pixel> pixels, ref_bytes;
    int w, h;
    get_w_h(file, w, h);
    open_pic(file, pixels, w, h);

    ofstream myfile;
    myfile.open("output.ppm");
    myfile << 'P' << '6' << '\n' << w << '\n' << h << '\n' << 255 << '\n';
    int o1 = 0, o2 = 0, o3 = 0, o4 = 0, o5 = 0;
    for (int i = 0; i < w; i++) {
        double y = round(0.2126 * pixels[i].r + 0.7152 * pixels[i].g + 0.0722 * pixels[i].b);
        if (y <= 50) {
            o1++;
        } else if (y <= 101) {
            o2++;
        } else if (y <= 152) {
            o3++;
        } else if (y <= 203) {
            o4++;
        } else {
            o5++;
        }
        myfile << pixels[i];
    }
    file.close();

    for (int c = 0; c < w * h - 2 * w; c++) {//pixels.size()
        if (c % w == 0 || c % w == w - 1) {
            Pixel p(pixels[c + w].r, pixels[c + w].g, pixels[c + w].b);
            myfile << p;
            double y = round(0.2126 * pixels[c + w].r + 0.7152 * pixels[c + w].g + 0.0722 * pixels[c + w].b);
            if (y <= 50) {
                o1++;
            } else if (y <= 101) {
                o2++;
            } else if (y <= 152) {
                o3++;
            } else if (y <= 203) {
                o4++;
            } else {
                o5++;
            }
            continue;
        }

        int n = pixels[c + w - 1].r * m.a1 + pixels[c].r * m.a1 + pixels[c + w].r * m.a2
                + pixels[c + 2 * w].r * m.a1 + pixels[c + w + 1].r * m.a1;
        unsigned char r = 255 > n ? n : 255;
        r = 0 < n ? r : 0;
        n = pixels[c + w - 1].g * m.a1 + pixels[c].g * m.a1 + pixels[c + w].g * m.a2
            + pixels[c + 2 * w].g * m.a1 + pixels[c + w + 1].g * m.a1;
        unsigned char g = 255 > n ? n : 255;
        g = 0 < n ? g : 0;
        n = pixels[c + w - 1].b * m.a1 + pixels[c].b * m.a1 + pixels[c + w].b * m.a2
            + pixels[c + 2 * w].b * m.a1 + pixels[c + w + 1].b * m.a1;
        unsigned char b = 255 > n ? n : 255;
        b = 0 < n ? b : 0;

        myfile << r << g << b;
        double y = round(0.2126 * r + 0.7152 * g + 0.0722 * b);
        if (y <= 50) {
            o1++;
        } else if (y <= 101) {
            o2++;
        } else if (y <= 152) {
            o3++;
        } else if (y <= 203) {
            o4++;
        } else {
            o5++;
        }
    }

    for (int i = w * h - w; i < w * h; i++) {
        double y = round(0.2126 * pixels[i].r + 0.7152 * pixels[i].g + 0.0722 * pixels[i].b);
        if (y <=50) {
            o1++;
        } else if (y <= 101) {
            o2++;
        } else if (y <= 152) {
            o3++;
        } else if (y <= 203) {
            o4++;
        } else {
            o5++;
        }
        myfile << pixels[i];
    }
    myfile.close();
    ofstream out_txt;
    out_txt.open("output.txt");
    out_txt << o1 << ' ' << o2 << ' ' << o3 << ' ' << o4 << ' ' << o5;
    out_txt.close();
    return 0;
}