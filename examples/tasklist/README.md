# Tasklist example
The tasklist example demonstrates a simple app that shows Cortex integration with Meteor/DDP. 

## Run the example
Ensure that [Meteor](https://www.meteor.com/install), [Cortex](http://www.github.com/cortexlang/cortex) and [web](http://www.github.com/cortexlang/web)
are properly installed on your machine (check the corresponding README's). Perform the following steps to run the example:

1. Open a console window
2. Run `source configure` from the Cortex installation directory
3. Go to the `<web installation directory>/examples/tasklist` directory
4. Run `meteor run &`
5. Run `cxsh ../../bin/libweb.so tasklist.cx`
6. Open `http://localhost:3000` in your webbrowser

Here are some cxsh commands (this is in fact the cortex scripting language) to modify tasks. Type `cd Tasklist` to move cxsh
to the Tasklist scope. Then run any of the followig commands:

### Create a new task
```
string anotherTask: "Something to do"
```

### Modify an existing task
```
anotherTask := "Something different to do"
```

### Delete a task
```
anotherTask.delete()
```
