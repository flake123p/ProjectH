
#
# subprocess() is better, popen() is easier
#
import os

cmd = 'dir'
fp = os.popen(cmd)

res = fp.read()

print('print(res) ... ')
print(res)

stat = fp.close()
print('print(stat) ... ')
print(stat)