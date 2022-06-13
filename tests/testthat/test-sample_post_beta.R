test_that("Return a value", {
  expect_equal(length(sample_post_beta(x = c(1,2,3,4),y = 4*c(1,2,3,4),tau = 1,tau_beta = 10)), 1)
})
