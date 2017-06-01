#!/usr/local/bin/Rscript
args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
  stop("Usage: drawGraphs.R csv-file", call.=FALSE)
} 

#Read data
csv <- read.csv(file=args[1], head=TRUE, sep=",")
samples = length(csv[,1])

file_suffix = paste(format(Sys.time(), "%d_%b_%Y-%H-%M-%S"), '.png', sep="")

img_h = 960
img_w = 960
header_offset = 4
#Read timestamp and cte value from csv file
time = as.numeric(as.character(csv[header_offset:samples,1])) 
time = time - time[1]
cte = as.numeric(as.character(csv[header_offset:samples,2]))
speed = as.numeric(as.character(csv[header_offset:samples,3]))
angle = as.numeric(as.character(csv[header_offset:samples,4]))/25.0
steering = as.numeric(as.character(csv[header_offset:samples,5]))

png(paste('plot_cte_', file_suffix, sep=""), width = img_w, height = img_h)
plot(time, cte, type="l")
lines(c(0,max(time)), c(0,0), col="red")
title("CTE graph")
# Add text using the following Corner_text function:
Corner_text <- function(text, location="topright"){
legend(location,legend=text, bty ="n", pch=NA) }
Corner_text(text=paste("Kp: ", toString(csv[1,3]), "\nKi: ",  toString(csv[1,5]), "\nKd: ",  toString(csv[1,7])))

dev.off()

png(paste('plot_angle_diff_time_', file_suffix, sep=""), width = img_w, height = img_h)
plot(time, abs(angle-steering), type="l")

dev.off()

png(paste('plot_angle_diff_speed_', file_suffix, sep=""), width = img_w, height = img_h)
plot(speed, abs(angle-steering), type="p")
dev.off()