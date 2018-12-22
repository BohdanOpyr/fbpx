import fbpx, time
for x in range(1920):
    fbpx.px(x, 540, 250, 0, 0)
fbpx.fill(0, 500, 1920, 580, 0, 250, 0)
time.sleep(1)
fbpx.fill(0, 0, 1920, 1080, 0, 0, 255)
for y in range(0, 1080, 20):
    fbpx.fill(0, y, 1920, y+10, 0, 255, 255)
