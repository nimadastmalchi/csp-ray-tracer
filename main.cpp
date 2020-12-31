/*

The following commands can be pasted onto Gnuplot after running the program.

splot '~miss_panel.txt' with vectors lw 1 lt 5, '~hit_panel.txt' with vectors lw 1 lt 10, '~hit_collector.txt' with vectors lw 1 lt 10, '~miss_collector.txt' with vectors lw 1 lt 1, '~collector_lines.txt' with lines lt -1, '~panel_lines.txt' with vectors nohead lw 1 lt - 1 , '~sun_path.txt' with vectors nohead lw 1 lt -1

splot '~hit_collector.txt' with vectors lw 1 lt 10, '~miss_collector.txt' with vectors lw 1 lt 1, '~collector_lines.txt' with lines lt -1, '~panel_lines.txt' with vectors nohead lw 1 lt - 1 , '~sun_path.txt' with vectors nohead lw 1 lt -1

splot '~panel_lines.txt' with vectors nohead lw 1 lt - 1

splot '~sun_path.txt' with vectors nohead lw 1 lt -1

plot '~var_sun_power.txt' with point pt 7 smooth csplines

plot '~var_sun_temp.txt' with point pt 7 smooth csplines

plot '~var_sun_inc_power.txt' with point pt 7 smooth csplines

plot '~var_sun_power_fixed.txt' with point pt 7 smooth csplines

plot '~var_sun_temp_fixed.txt' with point pt 7 smooth csplines

*/

#include "RayTracer.h" // includes Ray.h (Position.h and Components.h), <iostream>, <math.h>, <vector>, <fstream>, <string>, <iomanip>

using namespace std;

void setInput(ifstream& input, float* inputArray, const int& numOfInputs);

int main() {
    cout << setprecision(4) << fixed;
    ifstream input("~input_polar.txt");
    float inputArray[13];

    setInput(input, inputArray, 13);

    float time = inputArray[0];
    Point colLoc(inputArray[1], inputArray[2], inputArray[3]);
    Point colDim(inputArray[4], inputArray[5], inputArray[6]);
    int N = inputArray[7];
    float rMin = inputArray[8], rMax = inputArray[9];
    float panelSize = inputArray[10];
    float panelDist = inputArray[11];
    float zInc = inputArray[12];

    RayTracer r(time, colLoc, colDim, N, rMin, rMax, panelSize, panelDist, zInc);
    r.setup(SETUP_MODE);
    r.generate();
    r.info(); cout << endl;
    r.visualize();
    r.setPanelContributions();
    r.printPanelData();

    //printVarSunData(SUNRISE + 0.1, SUNSET, 0.01, colLoc, colDim, N, rMin, rMax, panelSize, panelDist, zInc);
    //printVarSunDataIncPolar(11, 13, 0.005, 0.5, colLoc, colDim, N, rMin, rMax, panelSize, panelDist, zInc);
    //printVarSunDataFixed(13, 15, 0.001, colLoc, colDim, N, rMin, rMax, panelSize, panelDist, zInc);

    input.clear();
    input.seekg(0, ios::beg);
}

void setInput(ifstream& input, float* inputArray, const int& numOfInputs) {
    string temp;
    int i = 0;
    while (i < numOfInputs) {
        input >> temp;
        try {
            inputArray[i] = stof(temp);
            i++;
        }
        catch (invalid_argument error) {}
    }
}