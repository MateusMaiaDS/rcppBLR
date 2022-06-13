#include <Rcpp.h>
#include <vector>
using namespace Rcpp;
using namespace std;

// // [[Rcpp::export]]
// double sample_post_beta(NumericVector x, NumericVector y, double tau, double tau_beta) {
//
//   // Storing the variables
//   int n = x.size();
//   double sum_x_times_y = 0;
//
//   // Calculating \sum_{i=1}^{N} x_{i}*y_{i}
//   for(int i = 0;i<n;i++){
//     sum_x_times_y = sum_x_times_y+(x[i]*y[i]);
//   }
//
//   return R::rnorm((sum_x_times_y/n + tau_beta/(tau*n)), sqrt(1/(tau*n)));
// }

