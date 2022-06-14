#' @useDynLib rcppBLR
#' @importFrom Rcpp sourceCpp

## BLM
#' @method predict "blm"
#' @rdname blm The posterior sampled models
#' @param object The posterior sampled paramters
#' @param x_new The new observations to sample new y
#' \method{predict}{blm}{object,
#' x_test}
#' @export
#'
# # Getting the values
predict.blm <- function(object,x_new){

  if(!class(object)=="blm")stop("Input must be a blm() model object.")

  chains <- list()
  for(i in 1:object$n_chain){
    # Retrieving the chains
    chains[[i]] <- y_post(x_new = x_new,beta_post = object$beta_post[,i],tau_post = object$tau_post[,i])
  }

  # Giving names to the chains
  names(chains) <- paste0("chain.",1:object$n_chain)

  return(chains)

}

