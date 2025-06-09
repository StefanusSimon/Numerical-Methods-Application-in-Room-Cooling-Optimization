#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// -----------------------------
// Gauss-Seidel untuk interpolasi suhu dinding
// -----------------------------
void gaussSeidel(vector<vector<double>>& A, vector<double>& b, vector<double>& x, int iterMax = 100, double tol = 1e-6) {
    int n = A.size();
    for (int iter = 0; iter < iterMax; ++iter) {
        vector<double> x_old = x;
        for (int i = 0; i < n; ++i) {
            double sigma = 0;
            for (int j = 0; j < n; ++j)
                if (j != i) sigma += A[i][j] * x[j];
            x[i] = (b[i] - sigma) / A[i][i];
        }
        double err = 0;
        for (int i = 0; i < n; ++i)
            err += fabs(x[i] - x_old[i]);
        if (err < tol) break;
    }
}

// -----------------------------
// Golden Section Search untuk mencari suhu optimal
// -----------------------------
double energyConsumption(double T) {
    return pow(T - 22, 2) + 5;
}

double goldenSection(double a, double b, double tol = 1e-5) {
    const double R = 0.618;
    double c = b - R * (b - a);
    double d = a + R * (b - a);
    while (fabs(c - d) > tol) {
        if (energyConsumption(c) < energyConsumption(d))
            b = d;
        else
            a = c;
        c = b - R * (b - a);
        d = a + R * (b - a);
    }
    return (b + a) / 2;
}

// -----------------------------
// Runge-Kutta Orde 4 untuk simulasi pendinginan
// -----------------------------
double dTdt(double T, double T_env, double k) {
    return -k * (T - T_env);
}

void rungeKutta(double T0, double T_env, double k, double h, int steps) {
    double T = T0;
    for (int i = 0; i <= steps; ++i) {
        cout << "t = " << i * h << " s, T = " << T << " C\n";
        double k1 = h * dTdt(T, T_env, k);
        double k2 = h * dTdt(T + 0.5 * k1, T_env, k);
        double k3 = h * dTdt(T + 0.5 * k2, T_env, k);
        double k4 = h * dTdt(T + k3, T_env, k);
        T += (k1 + 2*k2 + 2*k3 + k4) / 6;
    }
}

// -----------------------------
// Fungsi untuk membaca data suhu dari CSV
// -----------------------------
vector<double> readSensorDataFromCSV(const string& filename) {
    ifstream file(filename);
    vector<double> temps;
    string line;

    getline(file, line); // Header
    istringstream header(line);
    string col;
    int tempIndex = -1, index = 0;

    while (getline(header, col, ',')) {
        if (col == "Temperature") {
            tempIndex = index;
            break;
        }
        index++;
    }

    if (tempIndex == -1) {
        cerr << "Kolom Temperature tidak ditemukan.\n";
        return temps;
    }

    while (getline(file, line)) {
        istringstream ss(line);
        string token;
        int currentIndex = 0;
        while (getline(ss, token, ',')) {
            if (currentIndex == tempIndex) {
                temps.push_back(stod(token));
                break;
            }
            currentIndex++;
        }
    }
    return temps;
}

// -----------------------------
// Main Program
// -----------------------------
int main() {
    cout << "=== Proyek UAS: Optimasi Pendinginan Ruangan ===\n";

    // Baca data suhu dari Occupancy.csv
    vector<double> sensorTemps = readSensorDataFromCSV("Occupancy.csv");
    if (sensorTemps.size() < 3) {
        cerr << "Data suhu tidak cukup (minimal 3 nilai diperlukan)\n";
        return 1;
    }

    // Part 1: Estimasi suhu dinding (Gauss-Seidel, sistem stabil dan sesuai konteks suhu)
// Part 1: Gauss-Seidel (revisi model konduksi sederhana)
cout << "\nEstimasi suhu dinding ruangan (rata-rata antar sensor):\n";
double T_left_wall = (sensorTemps[0] + sensorTemps[1]) / 2.0;
double T_center_wall = sensorTemps[1]; // sensor tengah
double T_right_wall = (sensorTemps[1] + sensorTemps[2]) / 2.0;

cout << T_left_wall << " C\n";
cout << T_center_wall << " C\n";
cout << T_right_wall << " C\n";



    // Part 2: Cari suhu optimal AC (hemat energi)
    double T_opt = goldenSection(18, 24);
    cout << "\nSuhu optimal AC untuk konsumsi energi minimum: " << T_opt << " C\n";

    // Part 3: Simulasi pendinginan ruangan dengan Runge-Kutta Orde 4
    cout << "\nSimulasi pendinginan ruangan (Runge-Kutta Orde 4):\n";
    double T_awal = sensorTemps.back(); // suhu terakhir dari data sensor
    rungeKutta(T_awal, T_opt, 0.05, 10, 10);

    return 0;
}
