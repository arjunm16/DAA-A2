///@file
/*
Group Details

|       Name            	|          ID            	|
|---------------------------|---------------------------|	    
| Harsh Vardhan Gupta     	|     2019B3A70630H 	    |
| Aryan Kapadia         	|     2019B3A70412H 	    |
| Arjun Muthiah  	        |     2019B3A70374H 	    |
*/
#include <bits/stdc++.h>
#include <chrono>
 
using namespace std;
#define pb push_back
 
/// @brief Equation of Line 
typedef struct lineqn {
    double a; ///<Slope of the Line 
    double b; ///< Intercept of Line 
    // variables in the eqn y = ax + b
};
 
/// @brief Cartesian Point coordinate (X, Y)
typedef struct point {
    double x; ///< X-coordinate
    double y; ///<Y-coordinate
    // cartesian point cord (x, y)
};

/// @brief Calculates the Squared Error for a given set of points and line 
/// @param pts Vector of points
/// @param l Line l
/// @return Returns the Squared Error 
double calcErr(vector<point> pts, lineqn l) {
    double err = 0;
    for(auto elt : pts) {
        double yi_est = l.a * elt.x + l.b;
        err += ((elt.y - yi_est) * (elt.y - yi_est));
    }
    return err;
}
 
/// @brief Calculated the line of best fit from set of points from i to j. Also, calculates the sqaured error of set of points wrt the best fit line calculated previously
/// @param pts Vector of points
/// @param L Vector of Vector of having Line Equation of the form y = ax + b
/// @param E Vector of Vector containing error E[i][j]
void precalc(vector<point> pts, vector<vector<lineqn>> &L, vector<vector<double>> &E) {
    
    vector<double> sum_xiyi(pts.size(), 0), sum_xi(pts.size(), 0), sum_yi(pts.size(), 0), sum_xi2(pts.size(), 0);
    for(int i = 0; i < pts.size(); i++) {
        sum_xi[i] = sum_xi[i - 1 < 0 ? 0 : i - 1] + pts[i].x;
        sum_yi[i] = sum_yi[i - 1 < 0 ? 0 : i - 1] + pts[i].y;
        sum_xiyi[i] = sum_xiyi[i - 1 < 0 ? 0 : i - 1] + pts[i].x * pts[i].y;
        sum_xi2[i] = sum_xi2[i - 1 < 0 ? 0 : i - 1] + pts[i].x * pts[i].x;
    }
    // sum_xi[0] = sum_yi[0] = sum_xiyi[0] = sum_xi2[0] = 0;
 
    for(int i = 0; i < pts.size(); i++) {
        for(int j = i + 1; j < pts.size(); j++) {
            double curr_x, curr_y, curr_xy, curr_x2, n;
            if(i - 1 < 0){
                curr_x = sum_xi[j];
                curr_y = sum_yi[j];
                curr_xy = sum_xiyi[j];
                curr_x2 = sum_xi2[j];
 
            }
            else {
                curr_x = sum_xi[j] - sum_xi[i - 1];
                curr_y = sum_yi[j] - sum_yi[i - 1];
                curr_xy = sum_xiyi[j] - sum_xiyi[i - 1];
                curr_x2 = sum_xi2[j] - sum_xi2[i - 1];
            }
            n = j - i + 1;
 
            double a, b;
            // y = ax + b
            a = ((n * curr_xy) - (curr_x * curr_y)) / ((n * curr_x2) - (curr_x * curr_x));
            b = (curr_y - (a * curr_x)) / n;
            // b = ((sum_yi * sum_xi2) - (sum_xi * sum_xiyi)) / ((n * sum_xi2) - (sum_xi * sum_xi));
            lineqn coef = {a, b};
            L[i][j] = coef;
            // E[i][j] = 
 
            vector<point> ans(n);
            ans.assign(pts.begin() + i, pts.begin() + j + 1);
            // cout << ans.size() << " " << n << "\n";
            //ans contains the points whose error wrt their corresponding best fit line eqn is to be calculated
            E[i][j] = calcErr(ans, L[i][j]);
 
        }
    }
}
 
// - * * * * 
// - - * * * 
// - - - * * 
// - - - - * 
// - - - - - 
// * is where our content is, i.e. always, col > row


/// @brief Runs the SLS algorithm and stores the answer (Least total penalty as per SLS) in the vector M. M[i] = OPT(i)
/// @param pts  Vector of points
/// @param n    SLS needs to figure out the best fit lines till index n of the points passed in the points vector
/// @param L    Vector of Vector of having Line Equation of the form y = ax + b
/// @param E    Vector of Vector containing error E[i][j]
/// @param M    Vector storing the optimal errors/penalties
/// @param C    Penalty for adding a line segment
/// @param M_idx
void SLS_helper(vector<point> pts, int n, vector<vector<lineqn>> L, vector<vector<double>> E, vector<double> &M, double C, vector<int> &M_idx) {
    vector<double> minVal;
    vector<int> minIndex;
    if(n != 0) {
        for(int j = 1; j < pts.size(); j++) {
            double min_pen = DBL_MAX;
            int min_idx = -1;
            for(int i = 1; i < j; i++) {
                double pen = E[i][j] + C + M[i - 1];
                if(pen < min_pen) {
                    min_pen = pen;
                    min_idx = i;
                }
            }
            M.pb(min_pen);
            M_idx.pb(min_idx);
        }
    }
}//iterative
 
/// @brief  Runs the SLS algorithm and stores the answer (Least total penalty as per SLS) in the vector M. M[i] = OPT(i)
/// @param pts Vector of points
/// @param j SLS needs to figure out the best fit lines till index n of the points passed in the points vector
/// @param L Vector of Vector of having Line Equation of the form y = ax + b
/// @param E  Vector of Vector containing error E[i][j]
/// @param M  Vector storing the optimal errors/penalties
/// @param outfile File for storing the Output
/// @param C Penalty for adding each segment
void SLS(vector<point> pts, int j, vector<vector<lineqn>> L,vector<vector<double>> E, vector<double> &M, ofstream &outfile, double C) {
    if(j != 0) {
        double min_pen = DBL_MAX;
        int min_idx = -1;
        for(int i = 1; i < j; i++) {
            
            double pen = E[i][j] + C + M[i - 1];
            if(pen < min_pen) {
                min_pen = pen;
                min_idx = i;
            }
        }
        // outfile << min_idx << " " << j << " " << L[min_idx][j].a << " " << L[min_idx][j].b << "\n";
        outfile << j << " " << L[min_idx][j].a << " " << L[min_idx][j].b << "\n";
        
        SLS(pts, min_idx - 1, L, E, M, outfile, C);
    }
}

/// @brief Driver function for Running Functions
/// @param argc Command line arguments
/// @param argv Command line arguments
/// @return 
int main(int argc, char* argv[]) {
    ifstream fin("ip.txt");
    int num;
    vector<point> pts;
    fin >> num;
    for(int i = 0; i < num; i++) {
        double x_coord, y_coord;
        fin >> x_coord;
        fin >> y_coord;
        pts.pb(point{x_coord, y_coord});
    }
 
    fin.close();
 
    vector<vector<lineqn>> Lpre(pts.size(), vector<lineqn> (pts.size(), {0, 0}));
    vector<vector<double>> Epre(pts.size(), vector<double> (pts.size(), 0));
    
    // cout << "Starting Precalc\n";
    
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    precalc(pts, Lpre, Epre);
 
    // cout << "Precalc Done\n";
 
    vector<double> M;
    vector<int> M_idx;
    M.pb(0);
    M_idx.pb(0);
    double C = atof(argv[1]);
 
    ofstream fout("op.txt");
 
    // cout << "\nstarting helper\n";

    SLS_helper(pts, pts.size() - 1, Lpre, Epre, M, C, M_idx);
 
    // cout << "\nfinished helper\n";
 
    // cout << "\nstarting recursive\n";
 
    // cout << "\n" << M[pts.size() - 1] << "\n";
 
    SLS(pts, pts.size() - 1, Lpre, Epre, M, fout, C);
    // SLS_iter(pts, pts.size() - 1, Lpre, Epre, M, fout, C);
 
    // cout << "\nfinished recursive\n";
 
    // cout << "\n";

    // for(auto elt : M_idx)
    //     cout << elt << " ";

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time elapsed = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();// << "[µs]" << std::endl;
 
    fout.close();
}