
if (Meteor.isClient) {
  var remote = DDP.connect("http://127.0.0.1:8000");
  var Tasklist = new Meteor.Collection("Tasklist", remote);
  remote.subscribe("Tasklist");

  Template.body.helpers({
    tasks: function () {
      return Tasklist.find();
    }
  });

  Template.body.events({
    "submit #new-task": function (event) {
      var desc = event.target["taskdesc"].value
      var name = "task_" + parseInt(Math.random() * 1000, 0).toString();
      Tasklist.insert({
          _id: "::Tasklist::" + name,
          meta: {
              "name": name,
              parent: "::Tasklist",
              type: "::corto::lang::string"
          },
          value: desc
      });
      event.target["taskdesc"].value = "";
      return false;
    }
  });

  Template.task.events({
    "click .delete": function (event) {
      Tasklist.remove(this._id);
    },
    "click .edit": function (event) {
      var description = window.prompt("Change the description of the task", this.value);
      // Tasklist.update(this._id, {$set: {value: description}});
      Tasklist.update(this._id, {value: description});
    }
  });

}
