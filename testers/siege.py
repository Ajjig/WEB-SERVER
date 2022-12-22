from siege import Siege

# Create a Siege object
s = Siege()

# Add a URL to the test list
s.urls.append('http://localhost:80')

# Set the number of concurrent users and the duration of the test
s.concurrent = 10
s.time = 60

# Run the test
s.run()

# Print the results
print(s.results)