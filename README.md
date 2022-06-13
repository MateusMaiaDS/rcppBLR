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
