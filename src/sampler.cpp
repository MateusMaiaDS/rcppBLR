#include <Rcpp.h>
#include <vector>
using namespace Rcpp;
using namespace std;

// [[Rcpp::export]]
double sample_post_beta(NumericVector x, NumericVector y, double tau, double tau_beta) {

  // Storing the variables
  int n = x.size();
  double sum_x_times_y = 0;
  double sum_x_squared = 0;
  // Calculating \sum_{i=1}^{N} x_{i}*y_{i}
  for(int i = 0;i<n;i++){
    sum_x_squared += (x[i]*x[i]);
    sum_x_times_y += (x[i]*y[i]);
  }


  return R::rnorm((tau*sum_x_times_y)/(sum_x_squared*tau+tau_beta),sqrt(1/(sum_x_squared*tau+tau_beta)));
}


// [[Rcpp::export]]
double sample_post_tau(NumericVector x, NumericVector y, double beta, double a_tau, double d_tau) {

  // Storing the variables
  int n = x.size();
  double residuals_squared_sum = 0;

  // Calculating \sum_{i=1}^{N} x_{i}*y_{i}
  for(int i = 0;i<n;i++){
    residuals_squared_sum += (y[i]-x[i]*beta)*(y[i]-x[i]*beta);
  }


  return R::rgamma((a_tau+n*0.5),1/(0.5*residuals_squared_sum+d_tau));
}

// Returning an array instead
// [[Rcpp::export]]
double sample_post_beta_arr(NumericVector x, NumericVector y, int n,
                            double tau, double tau_beta) {

  // Storing the variables
  double sum_x_times_y = 0;
  double sum_x_squared = 0;
  // Calculating \sum_{i=1}^{N} x_{i}*y_{i}
  for(int i = 0;i<n;i++){
    sum_x_squared += (x[i]*x[i]);
    sum_x_times_y += (x[i]*y[i]);
  }


  return R::rnorm((tau*sum_x_times_y)/(sum_x_squared*tau+tau_beta),sqrt(1/(sum_x_squared*tau+tau_beta)));
}


// [[Rcpp::export]]
double sample_post_tau_arr(NumericVector x, NumericVector y, int n,
                           double beta, double a_tau, double d_tau) {

  // Storing the variables
  double residuals_squared_sum = 0;

  // Calculating \sum_{i=1}^{N} x_{i}*y_{i}
  for(int i = 0;i<n;i++){
    residuals_squared_sum += (y[i]-x[i]*beta)*(y[i]-x[i]*beta);
  }


  return R::rgamma((a_tau+n*0.5),1/(0.5*residuals_squared_sum+d_tau));
}



// [[Rcpp::export]]
List gibbs_sampler_chain(NumericVector x,
                         NumericVector y,
                         int n_mcmc,
                         int n_burn,
                         int n_chain,
                         double tau_beta, double a_tau,double d_tau) {

      int post_iter = 0;
      int n_post = n_mcmc-n_burn;
      double curr_beta ;
      double curr_tau ;

      int n = x.size();
      NumericMatrix beta_post(n_post, n_chain);
      NumericMatrix tau_post(n_post, n_chain);


      for(int j = 0; j < n_chain; j++){
        post_iter = 0;
        curr_beta = R::rnorm(0,10); // Random initialization
        curr_tau = R::runif(0,200); // Random initialization
        // curr_beta = 0;
        //curr_tau = 10;
          for(int i = 0;i<n_mcmc;i++){

            curr_beta = sample_post_beta_arr(x,y,n,curr_tau,tau_beta);
            curr_tau = sample_post_tau_arr(x,y,n,curr_beta,a_tau,d_tau);

           // Saving only the burn
             if(i>=n_burn){
               beta_post(post_iter,j) = curr_beta;
               tau_post(post_iter,j) = curr_tau;
               post_iter++;
              }

          }


        }
      // Defining the output
      return Rcpp::List::create(_["beta_post"] = beta_post,
                                _["tau_post"] = tau_post);


}

// [[Rcpp::export]]
NumericVector gamma_sampler(int n, double shape, double rate){

  NumericVector gamma_vec;
  for(int i = 0; i<n; i++){
  gamma_vec.push_back(R::rgamma(shape,1/rate));
  }
  return gamma_vec;
}

// [[Rcpp::export]]
List gibbs_sampler(NumericVector x,NumericVector y,
                   int n_mcmc,int n_burn,double init_beta,
                   double init_tau,double tau_beta, double a_tau,double d_tau) {

  double curr_beta = init_beta;
  double curr_tau = init_tau;

  NumericVector beta_post;
  NumericVector tau_post;


  for(int i = 0;i<n_mcmc;i++){

    curr_beta = sample_post_beta(x,y,curr_tau,tau_beta);
    curr_tau = sample_post_tau(x,y,curr_beta,a_tau,d_tau);


    if(i>=n_burn){
      beta_post.push_back(curr_beta);
      tau_post.push_back(curr_tau);
    }
  }


  // Defining the output
  return Rcpp::List::create(_["beta_post"] = beta_post,
                            _["tau_post"] = tau_post);


}

// [[Rcpp::export]]
List blm(NumericVector x, NumericVector y, int n_mcmc, int n_burn, int n_chain,
         double tau_beta = 0.001 ,double a_tau = 0.001, double d_tau = 0.001){

  // Calculating main aspects from the model
  int post_iter = 0;
  int n_post = n_mcmc - n_burn;
  int n = x.size();
  NumericMatrix tau_post(n_post,n_chain);
  NumericMatrix beta_post(n_post,n_chain);

  // MCMC elements
  double curr_beta,curr_tau;


  // Running the model
  for(int j = 0; j<n_chain;j++){

    // Defining the begin of each chain
    post_iter = 0;
    curr_beta = R::rnorm(0,10); // Random initialization
    curr_tau = R::runif(0,200); // Random initialization

      for(int i = 0; i<n_mcmc;i++){

        // Getting the values
        curr_beta = sample_post_beta_arr(x,y,n,curr_tau,tau_beta);
        curr_tau = sample_post_tau_arr(x,y,n,curr_beta,a_tau,d_tau);

        // Storing the values
        if(i>=n_burn){
          tau_post(post_iter,j) = curr_tau;
          beta_post(post_iter,j) = curr_beta;
          post_iter++;
        }

      }
  }

  // Defining the output
  List output = Rcpp::List::create(_["beta_post"] = beta_post,
                            _["tau_post"] = tau_post,
                            _["n_post"] = n_post,
                            _["n_chain"] = n_chain);

  output.attr("class") = "blm";

  // Returning the output
   return output;

}


// [[Rcpp::export]]
RcppExport NumericVector y_post(NumericVector x_new,
                     NumericVector beta_post,
                     NumericVector tau_post){

  int n_new = x_new.size();
  int n_post = beta_post.size();


  NumericMatrix y_post(n_post,n_new);


  // Aux for n_chain iter
      for(int k = 0; k<n_new;k++){
        for(int i = 0; i<n_post;i++){
            y_post(i,k)=R::rnorm(x_new[k]*beta_post[i],sqrt(1/tau_post[i]));
        }
      }
  return y_post;
}

