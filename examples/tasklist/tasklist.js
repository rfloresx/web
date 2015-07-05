
if (Meteor.isClient) {
  var remote = DDP.connect("http://127.0.0.1:8000");
  var Tasklist = new Meteor.Collection("Tasklist", remote);
  remote.subscribe("Tasklist")

  Template.body.helpers({
    objects: function() {
      return Tasklist.find();
    }
  });
}

if (Meteor.isServer) {
  Meteor.startup(function () {
  });
}
