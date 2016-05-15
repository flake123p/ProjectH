
import anydbm

#
# 'c' means that the database should be created if it doesn't already exist.
#
db = anydbm.open('captions.db', 'c')

db['aaa'] = '111'

print('db[\'aaa\'] = ', db['aaa'])

#
# anydbm => keys and values have to be strings!! Use pickle
#