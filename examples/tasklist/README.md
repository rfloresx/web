# Tasklist example
The tasklist example demonstrates a simple app that shows Cortex integration with Meteor/DDP. 

## Run the example
Ensure that the latest versions of [Meteor](https://www.meteor.com/install), [Cortex](http://www.github.com/cortexlang/cortex) and [web](http://www.github.com/cortexlang/web)
are properly installed on your machine (check the README's of cortex and web). Perform the following steps to run the example:

1. Open a console window
2. Run `source configure` from the Cortex installation directory
3. Go to the `<web installation directory>/examples/tasklist` directory
4. Run `meteor run &`
5. Run `cxsh ../../bin/libweb.so tasklist.cx`
6. Open `http://localhost:3000` in your webbrowser

### Modify the list
Here are some cxsh commands (this is in fact the cortex scripting language) to modify tasks. Type `cd Tasklist` to move cxsh
to the Tasklist scope. Then run any of the followig commands:

#### Create a new task
```
string anotherTask: "Something to do"
```

#### Update an existing task
```
anotherTask := "Something different to do"
```

#### Delete a task
```
anotherTask.delete()
```

## Contents

### tasklist.js
Contains the Meteor Client & Server code. The client creates a DDP connection to the cortex server (`http://localhost:8000`) and subscribes for the `Tasklist` collection:
```JavaScript
  var remote = DDP.connect("http://127.0.0.1:8000");
  var Tasklist = new Meteor.Collection("Tasklist", remote);
  remote.subscribe("Tasklist")
```
The JS file also contains a simple helper that enables retrieving the Tasklist objects from HTML.
```JavaScript
  Template.body.helpers({
    objects: function() {
      return Tasklist.find();
    }
  });
```

### tasklist.html
Contains a Meteor (Blaze) template that expands into the tasklist, which is populated from DDP. The template generates a list item for each entry in the Tasklist collection, with as content the `value` member of the JS object:
```HTML
    {{#each objects}}
      <li>{{value}}</li>
    {{/each}}
```

### tasklist.cx
Contains the Tasklist collection, which is a plain Cortex scope:
```C++
void Tasklist::
    string task_01: "Go to tasklist directory"
    ...
```
This script also creates the DDP server:
```C++
web::DDPServer s: 8000
```

### tasklist.css
An empty stylesheet.
