import os 

print ("Content-type: text/html")

for k, v in os.environ.items():
    print("<H5>", end="")
    print(f"{k} = {v}", end="")
    print("</H5>", end="")