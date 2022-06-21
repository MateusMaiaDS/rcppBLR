# rcppBLR
rcpp file for a simple LR

# To download use

```r
# install.packages("devtools")
devtools::install_github("MateusMaiaDS/rcppBLR")
```
# A simple example

```r

# Test example
n <- 500
x <- rnorm(n = n)
y <- 10*x + rnorm(n = n,sd = 0.1)


blm_mod <- blm(x = x,y = y,n_mcmc = 2000,
                n_burn = 250,n_chain = 2,tau_beta = 0.01,
                a_tau = 0.01,d_tau = 0.01)
                
pred_blm <- predict(blm_mod,1:10)

```

# To a complete comparison

```r 

# Loading packages
library(rcppBLR)
library(R2jags)

# Test example
n <- 500
x <- rnorm(n = n)
y <- 10*x + rnorm(n = n,sd = 0.1)


# Jags code to fit the model to the simulated data

model_code <- "
model
{
  # Likelihood
  for (i in 1:n) {
    y[i] ~ dnorm( beta * x[i], sigma^-2)
  }
  # Priors
  beta ~ dnorm(0, 100^-2)
  sigma ~ dunif(0, 10)
}
"

# Set up the data
model_data <- list(n = n, y = y, x = x)

# Choose the parameters to watch
model_parameters <- c( "beta", "sigma")





benchmark_test <- microbenchmark::microbenchmark(# Run the model
  jags(
    data = model_data,
    parameters.to.save = model_parameters,
    model.file = textConnection(model_code),
    n.chains = 4, # Number of different starting positions
    n.iter = 1000, # Number of iterations
    n.burnin = 200, # Number of iterations to remove at start
    n.thin = 1
  ), # Amount of thinning
  blm(x = x,y = y,n_mcmc = 1000,n_burn = 200,n_chain = 4,
      tau_beta = 0.001,a_tau = 0.001,d_tau = 0.001)
)
benchmark_test %>% summary

```
