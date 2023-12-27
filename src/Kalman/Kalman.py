# Classes modeling the KF and system for simulation. 
import numpy as np

class KalmanFilter():
    def __init__(self,
                 n_inputs,
                 n_outputs,
                 n_states,
                 x_hat=None,
                 F=None,
                 B=None,
                 H=None,
                 Q=None,
                 R=None):
        
        self._n_inputs = n_inputs
        self._n_outputs = n_outputs
        self._n_states = n_states

        self.set_initial_state(x_hat)

        self.set_F(F)

        self.set_B(B)

        self.set_H(H)

        self.set_Q(Q)

        self.set_R(R)

        self._I = np.eye(n_states)

        self._P = np.copy(self._I)
        self._P_hat = np.copy(self._I)
        self._y = np.zeros_like(self._R)

    def set_initial_state(self,x_hat):
        if x_hat is None:
            self._x_hat = np.zeros((self._n_states,1))
        else:
            assert type(x_hat) == np.ndarray
            assert x_hat.shape[0] == self._n_states, f'x_hat.shape[0] must equal n_states, got {x_hat.shape[0]} expected {self._n_states}'
            assert len(x_hat.shape) == 2 and x_hat.shape[1] == 1
            self._x_hat = x_hat

        self._xp_hat = np.copy(self._x_hat)

    def set_F(self,F):
        if F is None:
            self._F = np.eye(self._n_states)
        else:
            assert type(F) == np.ndarray
            assert F.shape[0] == self._n_states, f'F.shape[0] must equal n_states, got {F.shape[1]} expected {self._n_states}'
            assert F.shape[1] == self._n_states, f'F.shape[1] must equal n_states, got {F.shape[1]} expected {self._n_states}'
            self._F = F

    def set_B(self,B):
        if B is None:
            self._B = np.ones((self._n_states, self._n_inputs))
        else:
            assert type(B) == np.ndarray
            assert B.shape[0] == self._n_states, f'B.shape[0] must equal n_states, got {B.shape[0]} expected {self._n_states}'
            assert B.shape[1] == self._n_inputs, f'B.shape[1] must equal n_inputs, got {B.shape[1]} expected {self._n_inputs}'
            self._B = B

    def set_H(self,H):
        if H is None:
            self._H = np.zeros((self._n_states,self._n_outputs))
        else:
            assert type(H) == np.ndarray
            assert H.shape[0] == self._n_inputs, f'H.shape[0] must equal n_inputs, got {H.shape[0]} expected {self._n_inputs}'
            assert H.shape[1] == self._n_states, f'H.shape[1] must equal n_states, got {H.shape[1]} expected {self._n_states}'
            self._H = H

    def set_Q(self,Q):
        if Q is None:
            self._Q = np.eye(self._n_states)
        else:
            assert Q.shape[0] == self._n_states, f'Q.shape[0] must equal n_states, got {Q.shape[0]} expected {self._n_states}'
            assert Q.shape[1] == self._n_states, f'Q.shape[1] must equal n_states, got {Q.shape[1]} expected {self._n_states}'
            self._Q = Q

    def set_R(self, R):
        if R is None:
            self._R=np.zeros((self._n_outputs,self._n_outputs))
        else:
            assert R.shape[0] == self._n_outputs, f'R.shape[0] must equal n_outputs, got {R.shape[0]} expected {self._n_outputs}'
            assert R.shape[1] == self._n_outputs, f'R.shape[1] must equal n_outputs, got {R.shape[1]} expected {self._n_outputs}'
            self._R = R

    def update(self,dt, z, u=None):
        if u is None:
            u=np.zeros_like(self._R)
        F = self._I+self._F*dt
        B = self._B*dt

        self._xp_hat = F @ self._x_hat + B @ u

        self._P_hat = F @ self._P @ F.T + self._Q

        S = self._H @ self._P_hat @ self._H.T + self._R

        K = self._P_hat @ self._H.T @ np.linalg.inv(S)

        self._P = (self._I - K @ self._H) @ self._P_hat

        self._y = z - self._H @ self._x_hat

        self._x_hat = self._xp_hat + K @ self._y

        return self._x_hat
    
class System():
    def __init__(self, A, B, C, D=None):
        self._A = A
        self._B = B
        self._C = C
        self._D = D
        self._I = np.eye(A.shape[0])
        self._x = np.zeros((A.shape[0],1))
        self._y = np.zeros((C.shape[0],1))
    def update(self, u, dt):
        Ad = self._I+self._A*dt
        Bd = self._B*dt
        self._x = Ad @ self._x + Bd @ u
        if self._D is None:
            self._y = self._C @ self._x
        else:
            self._y = self._C @ self._x + self._D @ u

        return self._y