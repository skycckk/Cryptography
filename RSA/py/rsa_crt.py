def rsa_crt():
	N1 = 5356488760553659
	N2 = 8021928613673473
	N3 = 56086910298885139
	N = [N1, N2, N3]
	
	C1 = 4324345136725864
	C2 = 2102800715763550
	C3 = 46223668621385973
	C = [C1, C2, C3]

	R = []
	for i in range(0, 3):
		R.append(mod_inverse(N[(i + 1) % 3] * N[(i + 2) % 3], N[i]))

	mul_n = 1
	x = 1
	for i in range(0, 3):
		x += (R[i] * N[(i + 1) % 3] * N[(i + 2) % 3] * C[i])
		mul_n *= N[i]

	x = x % mul_n
	m = round(x ** (1. / 3))

	print('-----------------------')
	print('C = M ** e mod N')
	for i in range(0, 3):
		print('C =', C[i], 'and', 'N =', N[i], ' ')
	print('Multiplicative =', x)
	print('M =', m)
	print('-----------------------')

def mod_inverse(a, m):
	m0 = m
	x0 = 0
	x1 = 1

	if m == 1:
		return 0

	while (a > 1):
		q = int(a / m)
		t = m
		m = a % m
		a = t
		t = x0
		x0 = (x1 - q * x0)
		x1 = t

	if x1 < 0:
		x1 = x1 + m0

	return x1
