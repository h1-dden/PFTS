firebase
  .database()
  .ref("Personnel data/")
  .on("value", function (snapshot) {
    var logs = "";
    snapshot.forEach(function(childSnapshot) {
      var childData = childSnapshot.val();
      logs += "<tr>";
      logs += "<td>" + childData.name + "</td>";
      logs += "<td>" + childData.vehicleid + "</td>";
      logs += "<td>" + childData.latitude + "</td>";
      logs += "<td>" + childData.longitude + "</td>";
      logs += "<td>" + childData.rfid + "</td>";
      logs += "<td>" + new Date(childData.timestamp).toLocaleString() + "</td>";
      logs += "</tr>";
    });
    document.getElementById("timestamp-logs").innerHTML = logs;
  });
