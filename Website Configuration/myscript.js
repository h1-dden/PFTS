var nameV, vehicleidV, latitudeV, longitudeV, rfidV;

function readForm() {
  nameV = document.getElementById("name").value;
  vehicleidV = document.getElementById("vehicle").value;
  latitudeV = document.getElementById("latitude").value;
  longitudeV = document.getElementById("longitude").value;
  rfidV = document.getElementById("rfid").value;
  console.log(nameV, vehicleidV, latitudeV, longitudeV, rfidV);
}

document.getElementById("insert").onclick = function () {
  readForm();

  firebase
    .database()
    .ref("Personnel data/" + rfidV)
    .set({
      name: nameV,
      vehicleid: vehicleidV,
      longitude: longitudeV,
      latitude: latitudeV,
      rfid: rfidV,
      timestamp: firebase.database.ServerValue.TIMESTAMP
    });

  alert("Data Inserted");
  document.getElementById("name").value = "";
  document.getElementById("vehicleid").value = "";
  document.getElementById("latitude").value = "";
  document.getElementById("longitude").value = "";
  document.getElementById("rfid").value = "";
};

document.getElementById("read").onclick = function () {
  readForm();

  firebase
    .database()
    .ref("Personnel data/" + rfidV)
    .on("value", function (snap) {
      document.getElementById("rfid").value = snap.val().rfid;
      document.getElementById("name").value = snap.val().name;
      document.getElementById("vehicle").value = snap.val().vehicleid;
      document.getElementById("longitude").value = snap.val().longitude;
      document.getElementById("latitude").value = snap.val().latitude;
      document.getElementById("timestamp").innerText = new Date(
        snap.val().timestamp
      ).toLocaleString();
    });
};

document.getElementById("update").onclick = function () {
  readForm();

  firebase
    .database()
    .ref("Personnel data/")
    .push({
      name: nameV,
      vehicleid: vehicleidV,
      longitude: longitudeV,
      latitude: latitudeV,
      rfid: rfidV,
      timestamp: firebase.database.ServerValue.TIMESTAMP
    });

  alert("Data Updated");
  document.getElementById("name").value = "";
  document.getElementById("vehicleid").value = "";
  document.getElementById("latitude").value = "";
  document.getElementById("longitude").value = "";
  document.getElementById("rfid").value = "";
};

document.getElementById("delete").onclick = function () {
  readForm();

  firebase
    .database()
    .ref("Personnel data/" + rfidV)
    .remove();

  alert("Data Deleted");
  document.getElementById("name").value = "";
  document.getElementById("vehicleid").value = "";
  document.getElementById("latitude").value = "";
  document.getElementById("longitude").value = "";
  document.getElementById("rfid").value = "";
};
