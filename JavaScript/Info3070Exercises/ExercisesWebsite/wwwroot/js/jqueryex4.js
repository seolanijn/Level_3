$(function () {    //main jQuery routine - executes every on page load

    const stringData =
        `[{ "id" : 123, "firstname": "Teachers", "lastname": "pet" },
          { "id" : 234, "firstname": "Brown", "lastname": "Nose" },
          { "id" : 345, "firstname": "Always", "lastname": "Late" }]`;


    //do we already have it loaded from a previous run in the current session?
    //if not load the start array to session storage now
    sessionStorage.getItem("studentData") === null ? sessionStorage.setItem("studentData", stringData) : null;


    //get the session data to an object format
    let data = JSON.parse(sessionStorage.getItem("studentData"));

    $("#loadbutton").click(e => {
        let html = "";
        data.map(student => {
            html += `<div id="${student.id}" class="list-group-item">${student.firstname},${student.lastname}</div>`;
        });
        $("#studentList").html(html);
        $("#loadbutton").hide();
        $("#addbutton").show();
        $("#removebutton").show();
    });  //loadbutton.click()

    $("#studentList").click(e => {
        const student = data.find(s => s.id === parseInt(e.target.id));
        $("#results").text(`you selected ${student.firstname}, ${student.lastname}`);
    });  //div click

    $("#addbutton").click(e => {

        //find the last student
        const student = data[data.length - 1];
        //add 101 to the id
        $("#results").text(`adding student ${student.id + 101}`);
        //add a new student to the object array
        data.push({ "id": student.id + 101, "firstname": "new", "lastname": "student" });
        //convert the object array back to a string and put it in session storage
        sessionStorage.setItem("studentData", JSON.stringify(data));
        let html = "";
        data.map(student => {
            html += `<div id= "${student.id}" 
                 class="list-group-item">${student.firstname},${student.lastname}
            </div>`;
        });
        $("#studentList").html(html);
    });

    $("#removebutton").click(e => {

        //make sure there is still data in the array
        if (data.length > 0) {

            const student = data[data.length - 1];
            data.splice(-1, 1);  //remove last entry in array
            $("#results").text(`removing student ${student.id}`);
            //put the updated data back to session storage
            sessionStorage.setItem("studentData", JSON.stringify(data));
            let html = "";
            data.map(student => {
                html += `<div id= "${student.id}" 
                 class="list-group-item">${student.firstname},${student.lastname}
            </div>`;
            });
            $("#studentList").html(html);
        } else {
            $("#results").text(`no students to remove`);
        }

    });
});
