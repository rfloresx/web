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
      return Tasklist.find({}, {
        sort: {
          "value.finished": 1,
          "value.date.year": 1,
          "value.date.month": 1,
          "value.date.day": 1,
          "value.description": 1
        }});
    }
  });

  Template.body.events({
    "change input": function (event) {
      $(event.target).parents("tr").addClass("modified");
    },
    "keypress td[contenteditable]": function (event) {
      var row = $(event.target).parents("tr");
      if (event.keyCode == 13) {
        row.find("button.update, button.insert").click();
        event.target.blur();
        return false;
      } else {
        if (!row.hasClass("insert")) {
          $(event.target).parents("tr").addClass("modified");
        }
      }
    },
    "click button.update": function (event) {
      var row = $(event.target).parents("tr");
      var _id = this._id;
      var desc = row.children("td.description").text();
      var day = parseInt(row.children("td.day").text());
      var month = parseInt(row.children("td.month").text());
      var year = parseInt(row.children("td.year").text());
      var finished = row.find("input[type=checkbox]").prop("checked") ? true : false;
      day = day ? day : 0;
      month = month ? month : 0;
      year = year ? year : 0;
      Tasklist.update(_id, {
        value: {
          description: desc,
          date: {day: day, month: month, year: year},
          finished: finished
        }});
      row.removeClass("modified");
      return false;
    },
    "click button.remove": function (event) {
      var _id = this._id;
      Tasklist.remove(_id);
      return false;
    },
    "click button.insert": function (event) {
      var row = $(event.target).parents("tr");
      var name = nextTaskName();
      var $desc = row.children("td.description")
      var $day = row.children("td.day");
      var $month = row.children("td.month");
      var $year = row.children("td.year");
      var desc = $desc.text();
      var day = parseInt($day.text());
      var month = parseInt($month.text());
      var year = parseInt($year.text());
      Tasklist.insert({
        _id: "::Tasklist::" + name,
        meta: {name: name, parent: "::Tasklist", type: "::Task"},
        value: {
          description: desc,
          date: {day: day, month: month, year: year},
          finished: false
        }
      });
      $desc.text("");
      $day.text("");
      $month.text("");
      $year.text("");
      return false;
    }
  });

}
