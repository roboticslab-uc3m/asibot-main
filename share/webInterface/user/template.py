from AsibotPy import CartesianClient
#######################################
home=[0,0,1.4,0,0]
#######################################
simCart = CartesianClient()
simCart.open('/ravebot')
# use '/canbot' for real
#######################################
print 'hello, robot!'
simCart.movl(home)  # defaults to 20 s
simCart.wait()      # wait for movement


#######################################
print 'done!'
simCart.close()

