from AsibotTask import *
########################################
home=[0,0,1.4,0,0]
########################################
simCart = CartesianClient()
simCart.open('/ravebot')  # '/canbot' for real
########################################
print "hi!"
simCart.movj(home)
Time.delay(5)


#######################################
print "bye!"
simCart.close()

