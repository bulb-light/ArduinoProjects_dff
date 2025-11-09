## Discrete PID controller approximation

Consider a continous PID controller:

$$u(t) = K_p e(t) + \frac{K_p}{T_i}\int_0^t e(t) dt + K_p T_d \frac{d}{dt}e(t)$$

Where $K_p$ is the proportional gain, $T_i$ the integral time, and $T_d$ the derivative time.

In order to discretize the PID controller, let's assume a discrete time $t = kT$, where $T$ is the sample
period and $k$ the discrete-time index ($\mathbb{Z}_{\ge 0}$).

$$u(kT) = K_p e(kT) + \frac{K_p}{T_i}\int_{0}^{kT} e(t) dt + K_p T_d \frac{d}{dt}e(kT)$$

Then, using the trapezoidal approximation of the integral term and the finite difference approximation of the derivative
term, the discrete form of the controller can be derived as:

$$u[kT] = K_p e[kT] + \frac{K_p}{T_i}\frac{T}{2}\left(e[0] + 2e[T] + ... + 2e[(k-1)T] + e[kT]\right) + K_p T_d \frac{e[kT] - e[(k-1)T]}{T}$$

Since $T$ is a constant, the independent variable is simply $k$, so we can rewrite the previous equation as follows:

$$u[k] = K_p e[k] + \frac{K_p}{T_i}\frac{T}{2}\left(e[0] + 2e[1] + ... + 2e[k-1] + e[k]\right) + K_p T_d \frac{e[k] - e[k-1]}{T}$$

For $k-1$, we get:

$$u[k-1] = K_p e[k-1] + \frac{K_p}{T_i}\frac{T}{2}\left(e[0] + 2e[1] + ... + 2e[k-2] + e[k-1]\right) + K_p T_d \frac{e[k-1] - e[k-2]}{T}$$

Subtracting the second equation from the first, we obtain:

$$u[k] - u[k-1] =  K_p e[k] - K_p e[k-1]  + \frac{K_p}{T_i}\frac{T}{2} \left( e[k-1] + e[k] \right) +  K_p T_d \frac{\left(e[k] - 2e[k-1] + e[k-2]\right)}{T}$$

Finally, the discrete-time PID is given by:

$$\boxed{u[k] = u[k-1] + q_0 e[k] + q_1 e[k-1] + q_2 e[k-2]}$$

Where:

$$q_0 = K_p \left( 1 + \frac{T}{2 T_i} + \frac{T_d}{T}\right)$$

$$q_1 = -K_p \left( 1 - \frac{T}{2 T_i} + \frac{2T_d}{T} \right)$$

$$q_2 = \frac{K_p T_d}{T}$$
