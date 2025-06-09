#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// ----------------------------------------------
// Fungsi Gauss-Seidel untuk menyelesaikan sistem persamaan linear
// A * x = b, digunakan untuk estimasi suhu dinding (interpolasi)
// ----------------------------------------------
void gaussSeidel(vector<vector<double>>& A, vector<double>& b, vector<double>& x, int iterMax = 100, double tol = 1e-6) {
    int n = A.size();
    for (int iter = 0; iter < iterMax; ++iter) {
        vector<double> x_old = x;
        for (int i = 0; i < n; ++i) {
            double sigma = 0;
            for (int j = 0; j < n; ++j)
                if (j != i) sigma += A[i][j] * x[j]; // Jumlah elemen baris selain diagonal
            x[i] = (b[i] - sigma) / A[i][i]; // Update nilai solusi
        }

        // Cek konvergensi dengan error total
        double err = 0;
        for (int i = 0; i < n; ++i)
            err += fabs(x[i] - x_old[i]);
        if (err < tol) break; // Iterasi berhenti jika error cukup kecil
    }
}

// ----------------------------------------------
// Fungsi objektif energi: meminimalkan konsumsi energi tergantung suhu AC
// Model: kuadrat selisih dari suhu ideal 22 derajat (fiktif) + konstan
// ----------------------------------------------
double energyConsumption(double T) {
    return pow(T - 22, 2) + 5;
}

// ----------------------------------------------
// Golden Section Search: Metode optimasi tanpa turunan
// Digunakan untuk mencari suhu optimal AC dengan konsumsi energi minimum
// ----------------------------------------------
double goldenSection(double a, double b, double tol = 1e-5) {
    const double R = 0.618; // rasio emas
    double c = b - R * (b - a);
    double d = a + R * (b - a);

    while (fabs(c - d) > tol) {
        if (energyConsumption(c) < energyConsumption(d))
            b = d;
        else
            a = c;

        // Update nilai c dan d
        c = b - R * (b - a);
        d = a + R * (b - a);
    }

    return (b + a) / 2; // Titik minimum perkiraan
}

// ----------------------------------------------
// Model pendinginan: hukum Newton (turunan suhu terhadap waktu)
// dT/dt = -k * (T - T_env), di mana k adalah konstanta pendinginan
// ----------------------------------------------
double dTdt(double T, double T_env, double k) {
    return -k * (T - T_env);
}

// ----------------------------------------------
// Simulasi pendinginan menggunakan Runge-Kutta Orde 4
// Digunakan untuk memprediksi perubahan suhu dari waktu ke waktu
// ----------------------------------------------
void rungeKutta(double T0, double T_env, double k, double h, int steps) {
    double T = T0;

    for (int i = 0; i <= steps; ++i) {
        cout << "t = " << i * h << " s, T = " << T << " C\n";

        // Perhitungan nilai k1 hingga k4 (RK4)
        double k1 = h * dTdt(T, T_env, k);
        double k2 = h * dTdt(T + 0.5 * k1, T_env, k);
        double k3 = h * dTdt(T + 0.5 * k2, T_env, k);
        double k4 = h * dTdt(T + k3, T_env, k);

        // Update suhu
        T += (k1 + 2*k2 + 2*k3 + k4) / 6;
    }
}

// ----------------------------------------------
// Membaca data suhu dari file CSV
// Kolom yang diambil: "Temperature"
// ----------------------------------------------
vector<double> readSensorDataFromCSV(const string& filename) {
    ifstream file(filename);
    vector<double> temps;
    string line;

    getline(file, line); // Ambil baris pertama (header)
    istringstream header(line);
    string col;
    int tempIndex = -1, index = 0;

    // Temukan indeks kolom "Temperature"
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

    // Ambil nilai suhu dari setiap baris
    while (getline(file, line)) {
        istringstream ss(line);
        string token;
        int currentIndex = 0;
        while (getline(ss, token, ',')) {
            if (currentIndex == tempIndex) {
                temps.push_back(stod(token)); // Konversi string ke double
                break;
            }
            currentIndex++;
        }
    }

    return temps;
}

// ----------------------------------------------
// MAIN PROGRAM
// ----------------------------------------------
int main() {
    cout << "=== Proyek UAS: Optimasi Pendinginan Ruangan ===\n";

    // Baca data suhu dari file Occupancy.csv
    vector<double> sensorTemps = readSensorDataFromCSV("Occupancy.csv");

    // Validasi: Minimal butuh 3 sensor untuk estimasi
    if (sensorTemps.size() < 3) {
        cerr << "Data suhu tidak cukup (minimal 3 nilai diperlukan)\n";
        return 1;
    }

    // --------------------------
    // Part 1: Estimasi suhu dinding
    // --------------------------
    // Sederhanakan dengan interpolasi linier antar sensor
    cout << "\nEstimasi suhu dinding ruangan (rata-rata antar sensor):\n";
    double T_left_wall   = (sensorTemps[0] + sensorTemps[1]) / 2.0; // rata-rata kiri
    double T_center_wall = sensorTemps[1];                          // langsung dari sensor tengah
    double T_right_wall  = (sensorTemps[1] + sensorTemps[2]) / 2.0; // rata-rata kanan

    // Tampilkan estimasi suhu dinding
    cout << T_left_wall << " C\n";
    cout << T_center_wall << " C\n";
    cout << T_right_wall << " C\n";

    // --------------------------
    // Part 2: Cari suhu optimal AC (efisiensi energi)
    // --------------------------
    double T_opt = goldenSection(18, 24); // Rentang suhu AC 18–24 derajat
    cout << "\nSuhu optimal AC untuk konsumsi energi minimum: " << T_opt << " C\n";

    // --------------------------
    // Part 3: Simulasi pendinginan ruangan
    // --------------------------
    cout << "\nSimulasi pendinginan ruangan (Runge-Kutta Orde 4):\n";

    double T_awal = sensorTemps.back(); // Ambil suhu terakhir sebagai awal
    rungeKutta(T_awal, T_opt, 0.05, 10, 10); // T0, T_env, k, langkah waktu, jumlah iterasi

    return 0;
}
