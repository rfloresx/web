if (Meteor.isClient) {
  var remote = DDP.connect("http://127.0.0.1:8000");
  var Tasklist = new Meteor.Collection("Tasklist", remote);
  remote.subscribe("Tasklist");

  Session.setDefault('nextTaskNumber', 0);

  var nextTaskName = function () {
    var n = Session.get('nextTaskNumber');
    var name = "t" + n.toString();
    Session.set('nextTaskNumber', n + 1);
    return name;
  };

  Template.body.helpers({
    tasks: function () {
      return Tasklist.find();
    }
  });

  Template.body.events({
    "submit #insert-task": function (event) {
      var desc = event.target["desc"].value;
      var day = parseInt(event.target["day"].value);
      var month = parseInt(event.target["month"].value);
      var year = parseInt(event.target["year"].value);
      event.target["desc"].value = "";
      event.target["day"].value = "";
      event.target["month"].value = "";
      event.target["year"].value = "";
      var name = nextTaskName();
      Tasklist.insert({_id: "::Tasklist::" + name,
        meta: {name: name, parent: "::Tasklist", type: "::Task"},
        value: {description: desc, date: {day: day, month: month, year: year}}});
      return false;
    },
    "submit #update-task": function (event) {
      var _id = event.target["_id"].value;
      var desc = event.target["desc"].value;
      var day = parseInt(event.target["day"].value);
      var month = parseInt(event.target["month"].value);
      var year = parseInt(event.target["year"].value);
      event.target["_id"].value = "";
      event.target["desc"].value = "";
      event.target["day"].value = "";
      event.target["month"].value = "";
      event.target["year"].value = "";
      Tasklist.update(_id, {value: {description: desc, date: {day: day, month: month, year: year}}});
      return false;
    },
    "submit #remove-task": function (event) {
      var _id = event.target["_id"].value;
      Tasklist.remove(_id);
      event.target["_id"].value = "";
      return false;
    }
  });

}
