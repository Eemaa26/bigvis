// Convolution options:
//   zero pad
//   truncuate
//   reflect
//   wrap

#include <Rcpp.h>
using namespace Rcpp;

//' 1d kernel smoothing
//'
//' @param x ordered vector of x positions
//' @param z vector of values
//' @param x_out vector of x positions to smooth for
//' @param sd standard deviation of normal kernel
//' @keywords internal
// [[Rcpp::export]]
NumericVector smooth_1d_normal(const NumericVector& x, const NumericVector& z, 
                        const NumericVector& x_out, const double sd) {

  int n_in = x.size(), n_out = x_out.size();
  NumericVector z_out(n_out);

  for (int i = 0; i < n_out; i++) {
    for (int j = 0; j < n_in; j++) {
      double dist = x[j] - x_out[i];
      // Only use middle 99% of normal kernel
      if (fabs(dist) > (4 * sd)) continue; 

      double k = R::dnorm(dist, 0.0, sd, 0);
      z_out[i] += z[j] * k;
    }
  }

  return z_out;
}


/*** R

  x <- as.numeric(1:10)
  z <- rep(c(1, 2), length = length(x))
  k <- kernel("norm", sd = 0.1)
  grid <- seq(0, 11, length = 1000)

  s <- smooth_1d_normal(x, z, grid, 0.1)
  plot(grid, s, type = "l", ylim = range(s, z))
  text(grid, min(s), seq_along(grid) - 1, cex = 0.5)
  text(x, z, seq_along(x) - 1)

  xs <- rep(x, z)
  plot(density(x, 0.1, weights = z))

*/