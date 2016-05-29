
#
# anydbm => keys and values have to be strings!! Use pickle
#
import pickle
t1 = [11, 22, 33]

#
# dumps = dump string
#
s = pickle.dumps(t1)
print('pickle.dumps(t1) = ', s)

t2 = pickle.loads(s)
print('pickle.loads(s) = ', t2)

print('t1 == t2 = ', t1 == t2)
print('t1 is t2 = ', t1 is t2)