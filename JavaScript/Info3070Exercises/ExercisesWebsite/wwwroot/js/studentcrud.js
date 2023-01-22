$(function () {

    const getAll = async (msg) => {
        try {
            $("#studentList").text("Finding Student Information...");
            let response = await fetch(`api/student`);
            if (response.ok) {
                let payload = await response.json();
                buildStudentList(payload);
                msg === "" ?
                    $("#status").text("Students Loaded") : $("#status").text(`${msg} -Students Loaded`);
            } else if (response.status !== 404) { // probably some other client side error
                let problemJson = await response.json();
                errorRtn(problemJson, response.status);
            } else {
                $("#status").text("no such path on server");
            }
            let response2 = await fetch(`api/division`);
            if (response2.ok) {
                let divs = await response2.json();
                sessionStorage.setItem("alldivisions", JSON.stringify(divs));
            } else if (response2.status !== 404) { // probably some other client side error
                let problemJson = await response2.json();
                errorRtn(problemJson, response2.status);
            } else {
                $("#status").text("no such path on server");
            }
        } catch (error) { // catastrophic
            $("#status").text(error.message);
        }
    };

    const loadDivisionDDL = (studiv) => {
        html = '';
        $('#ddlDivisions').empty();
        let alldivisions = JSON.parse(sessionStorage.getItem('alldivisions'));
        for (div of alldivisions) { html += `<option value="${div.id}">${div.name}</option>` };
        $('#ddlDivisions').append(html);
        $('#ddlDivisions').val(studiv);
    };

    const setupForUpdate = (id, data) => {
        $("#actionbutton").val("update");
        $("#deletebutton").show();
        $("#modaltitle").html("<h4>update student</h4>");

        clearModalFields();
        data.map(student => {
            if (student.id === parseInt(id)) {
                $("#TextBoxTitle").val(student.title);
                $("#TextBoxFirstname").val(student.firstname);
                $("#TextBoxLastname").val(student.lastname);
                $("#TextBoxPhone").val(student.phoneno);
                $("#TextBoxEmail").val(student.email);
                sessionStorage.setItem("id", student.id);
                sessionStorage.setItem("divisionId", student.divisionId);
                sessionStorage.setItem("timer", student.timer);
                $("#modalstatus").text("update data");
                loadDivisionDDL(student.divisionId);
                $("#myModal").modal("toggle");
                $("#myModalLabel").text("Update");
            }
        });
    };

    const setupForAdd = () => {
        $("#actionbutton").val("add");
        $("#deletebutton").hide();
        $("#modaltitle").html("<h4>add student</h4>");
        $("#theModal").modal("toggle");
        $("#modalstatus").text("add new student");
        $("#myModalLabel").text("Add");
        clearModalFields();
    };

    const clearModalFields = () => {
        loadDivisionDDL(-1);
        $("#TextBoxTitle").val("");
        $("#TextBoxFirstname").val("");
        $("#TextBoxLastName").val("");
        $("#TextBoxPhone").val("");
        $("#TextBoxEmail").val("");
        sessionStorage.removeItem("Id");
        sessionStorage.removeItem("divisionId");
        sessionStorage.removeItem("timer");
        $("#myModal").modal("toggle");
    };

    const add = async () => {
        try {
            stu = new Object();
            stu.title = $("#TextBoxTitle").val();
            stu.firstname = $("#TextBoxFirstname").val();
            stu.lastname = $("#TextBoxLastname").val();
            stu.phoneno = $("#TextBoxPhone").val();
            stu.email = $("#TextBoxEmail").val();
            stu.divisionId = parseInt($("#ddlDivisions").val());
            stu.id = -1;
            stu.timer = null;
            stu.picture64 = null;
            //
            let response = await fetch("api/student", {
                method: "POST",
                headers: { "Content-Type": "application/json; charset=utf-8" },
                body: JSON.stringify(stu)
            });
            if (response.ok) {
                let data = await response.json();
                getAll(data.msg);
            } else if (response.status !== 404) {
                let problemJson = await response.json();
                errorRtn(problemJson, response.status);
            } else {
                $("#status").text("no such path on server");
            }
        } catch (error) {
            $("#status").text(error.message);
        }
        $("#myModal").modal("toggle");
    };

    const update = async () => {
        try {
            stu = new Object();
            stu.title = $("#TextBoxTitle").val();
            stu.firstname = $("#TextBoxFirstname").val();
            stu.lastname = $("#TextBoxLastname").val();
            stu.phoneno = $("#TextBoxPhone").val();
            stu.email = $("#TextBoxEmail").val();
            stu.id = parseInt(sessionStorage.getItem("id"));
            stu.divisionId = parseInt($("#ddlDivisions").val());
            stu.timer = sessionStorage.getItem("timer");
            stu.picture64 = null;
            //
            let response = await fetch("api/student", {
                method: "PUT",
                headers: { "Content-Type": "application/json; charset=utf-8" },
                body: JSON.stringify(stu)
            });
            if (response.ok) {
                let data = await response.json();
                getAll(data.msg);
            } else if (response.status !== 404) {
                let problemJson = await response.json();
                errorRtn(problemJson, response.status);
            } else {
                $("#status").text("no such path on server");
            }
        } catch (error) {
            $("#status").text(error.message);
        }
        $("#myModal").modal("toggle");
    };

    const _delete = async () => {
        try {
            let response = await fetch(`api/student/${sessionStorage.getItem('id')}`, {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json; charset=utf-8' }
            });
            if (response.ok) {
                let data = await response.json();
                getAll(data.msg);
            } else {
                $('#status').text(`Status - ${response.status}, Problem on delete server side, see server console`);
            }
            $('#myModal').modal('toggle');
        } catch (error) {
            $('#status').text(error.message);
        }
    };

    $("#actionbutton").click(() => {
        $("#actionbutton").val() === "update" ? update() : add();
    });

    $("#deletebutton").click(() => {
        if (window.confirm('Are you sure'))
            _delete();
    });

    $("#studentList").click((e) => {
        if (!e) e = window.event;
        let id = e.target.parentNode.id;
        if (id === "studentList" || id === "") {
            id = e.target.id;
        }
        if (id !== "status" && id !== "heading") {
            let data = JSON.parse(sessionStorage.getItem("allstudents"));
            id === "0" ? setupForAdd() : setupForUpdate(id, data);
        } else {
            return false;
        }
    });


    const buildStudentList = (data) => {
        $("#studentList").empty();
        div = $(`<div class="list-group-item text-white bg-secondary row d-flex" id="status">Student Info</div>
                 <div class= "list-group-item row d-flex text-center" id="heading">
                 <div class="col-4 h4">Title</div>
                 <div class="col-4 h4">First</div>
                 <div class="col-4 h4">Last</div>
                 </div>`);
        div.appendTo($("#studentList"));
        sessionStorage.setItem("allstudents", JSON.stringify(data)); //169
        btn = $(`<button class="list-group-item row d-flex" id="0">...click to add student</button>`);
        btn.appendTo($("#studentList"));
        data.map(stu => {
            btn = $(`<button class="list-group-item row d-flex" id="${stu.id}">`);
            btn.html(`<div class="col-4" id="studenttitle${stu.id}">${stu.title}</div>
                      <div class="col-4" id="studentfname${stu.id}">${stu.firstname}</div>
                      <div class="col-4" id="studentlastname${stu.id}">${stu.lastname}</div>`
            );
            btn.appendTo($("#studentList"));
        });
    };

    getAll("");
});


const errorRtn = (problemJson, status) => {
    if (status > 499) {
        $("#status").text("Problem server side, see debug console");
    } else {
        let keys = Objects.keys(problemJson.errors)
        problem = {
            status: status,
            statusText: problemJson.errors[keys[0]][0],
        };
        $("#status").text("Problem client side, see browser console");
        console.log(problem);
    }
}


