from AsibotPy import *
#######################################
home=[0,0,1.4,0,0]
#######################################
simCart = CartesianClient()
simCart.open('/ravebot')
# use '/canbot' for real
#######################################
print 'hello, robot!'
simCart.movl(home)  # defaults to 20 s
Time.delay(20)


#######################################
print 'done!'
simCart.close()

