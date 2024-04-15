# Scan pattern design

Continuously scan all points spread from origin
Interpolate all points within threhold

## Portability

Current implementation is not very portable, but we do not care.

Only has to work for interface between 2 ARM/ststm microcontrollers, so both should be expected to use same float implementations.

A future better implementation would not do this!! If you are going to copycat learn from this unless you can also garuntee your float implementations are the same.

heed this warning
