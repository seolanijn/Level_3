/* filename: employee.js
 * description: it will make the user be able to interact with employee data
 */
$(function () {
    

    const getAll = async (msg) => {
        try {
            $("#employeeList").text("Finding Employee Information..");
            let response = await fetch(`api/employee`);
            if (response.ok) {
                let payload = await response.json(); // returns a promise
                buildEmployeeList(payload);
                msg === "" ?
                    $("#status").text("Employees Loaded") : $("#status").text(`${msg} - Employees Loaded`);
            } else if (response.status !== 404) { // probably some other client side error
                let problemJson = await response.json();
                errorRtn(problemJson, response.status);

            } else { // else 404 not found
                $("#status").text("no such path on server");
            }
            let response2 = await fetch(`api/department`);
            if (response2.ok) {
                let divs = await response2.json(); // returns a promise
                sessionStorage.setItem("alldepartments", JSON.stringify(divs));
            } else if (response2.status !== 404) { // probably some other client side error
                let problemJson = await response2.json();
                errorRtn(problemJson, response2.status);
            } else { // else 404 not found
                $("#status").text("no such path on server");
            }
        } catch (error) {
            $("#status").text(error.message);
        }
    }; // end getAll


    const loadDepartmentDDL = (empdep) => {
        html = '';
        $('#ddlDepartments').empty();
        let alldepartments = JSON.parse(sessionStorage.getItem('alldepartments'));
        for (dep of alldepartments) { html += `<option value="${dep.id}">${dep.name}</option>` }; // show departments name
        $('#ddlDepartments').append(html);
        $('#ddlDepartments').val(empdep);
    }; // end loadDepartmentDDL


    const setupForUpdate = (id, data) => {
        $("#actionbutton").val("update");
        $("#deletebutton").show();
        $("#modaltitle").html("<h4>update employee</h4>");

        clearModalFields();
        data.map(employee => {
            if (employee.id === parseInt(id)) {
                $("#TextBoxTitle").val(employee.title);
                $("#TextBoxFirstname").val(employee.firstname);
                $("#TextBoxLastname").val(employee.lastname);
                $("#TextBoxPhone").val(employee.phoneno);
                $("#TextBoxEmail").val(employee.email);
                sessionStorage.setItem("id", employee.id);
                sessionStorage.setItem("departmentId", employee.departmentId);
                sessionStorage.setItem("timer", employee.timer);
                $("#modalstatus").text("update data");
                loadDepartmentDDL(employee.departmentId);
                $("#myModal").modal("toggle");
                $("#myModalLabel").text("Update");
            }
        });
    }; // end setupForUpdate

    const setupForAdd = () => {
        $("#actionbutton").val("add");
        $("#deletebutton").hide();
        $("#modaltitle").html("<h4>add employee</h4>");
        $("#theModal").modal("toggle");
        $("#modalstatus").text("add new employee");
        $("#myModalLabel").text("Add");
        clearModalFields();
    }; // end setupForAdd



    const clearModalFields = () => {
        loadDepartmentDDL(-1);
        $("#TextBoxTitle").val("");
        $("#TextBoxFirstname").val("");
        $("#TextBoxLastname").val("");
        $("#TextBoxPhone").val("");
        $("#TextBoxEmail").val("");
        sessionStorage.removeItem("id");
        sessionStorage.removeItem("depertmentId");
        sessionStorage.removeItem("timer");
        $("#myModal").modal("toggle");
        $("#EmployeeModalForm").validate().resetForm();
    }; // end clearModalFields

    const add = async () => {
        try {
            emp = new Object();

            emp.title = $("#TextBoxTitle").val();
            emp.firstname = $("#TextBoxFirstname").val();
            emp.lastname = $("#TextBoxLastname").val();
            emp.phoneno = $("#TextBoxPhone").val();
            emp.email = $("#TextBoxEmail").val();
            emp.departmentId = parseInt($("#ddlDepartments").val());
            emp.id = -1;
            emp.timer = null;
            emp.picture64 = null;

            // send the student's information to the server by POST
            let response = await fetch("api/employee", {
                method: "POST",
                headers: { "Content-Type": "application/json; charset=utf-8" },
                body: JSON.stringify(emp)
            });

            if (response.ok) {
                let data = await response.json();
                $("#status").text(data.msg);
            } else if (response.status !== 404) { // some other client side error
                let problemJson = await response.json();
                errorRtn(problemJson, response.status);
            } else { // else 404 not found
                $("#status").text("no such path on server");
            }
        } catch (error) {
            $("#status").text(error.message);
        }
        $("#myModal").modal("toggle");
    }; // end add

    const update = async () => {
        try {
            // new employee
            emp = new Object();

            // new employee's properties
            emp.title = $("#TextBoxTitle").val();
            emp.firstname = $("#TextBoxFirstname").val();
            emp.lastname = $("#TextBoxLastname").val();
            emp.phoneno = $("#TextBoxPhone").val();
            emp.email = $("#TextBoxEmail").val();

            emp.id = parseInt(sessionStorage.getItem("id"));
            emp.departmentId = parseInt($("#ddlDepartments").val());
            emp.timer = sessionStorage.getItem("timer");
            emp.picture64 = null;

            // send to the server
            let response = await fetch("api/employee", {
                method: "PUT",
                headers: { "Content-Type": "application/json; charset=utf-8" },
                body: JSON.stringify(emp)
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
    }; // end update

    const _delete = async () => {
        try {
            let response = await fetch(`api/employee/${sessionStorage.getItem('id')}`, {
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
    }; // end _delete

    $("#actionbutton").click(() => {
        $("#actionbutton").val() === "update" ? update() : add();
    }); // end actionbutton click

    $("#deletebutton").click(() => {
        if (window.confirm('Are you sure'))
            _delete();
    }); // end deletebutton click

    $("#employeeList").click((e) => {
        if (!e) e = window.event;
        let id = e.target.parentNode.id;
        if (id === "employeeList" || id === "") {
            id = e.target.id;
        }
        if (id !== "status" && id !== "heading") {
            let data = JSON.parse(sessionStorage.getItem("allemployees"));
            id === "0" ? setupForAdd() : setupForUpdate(id, data);
        } else {
            return false;
        }
    }); // end employeeList click

    const buildEmployeeList = (data) => {
        $("#employeeList").empty();
        div = $(`<div class="list-group-item text-white bg-secondary row d-flex" id="status">employee Info</div>
                <div class="list-group-item row d-flex text-center" id="heading">
                <div class="col-4 h4">Title</div>
                <div class="col-4 h4">First</div>
                <div class="col-4 h4">Last</div>
                </div>`);
        div.appendTo($("#employeeList"));
        sessionStorage.setItem("allemployees", JSON.stringify(data));
        btn = $(`<button class="list-group-item row d-flex" id="0">...click to add employee</button>`);
        btn.appendTo($("#employeeList"));
        data.map(emp => {
            btn = $(`<button class="list-group-item row d-flex" id="${emp.id}">`);
            btn.html(`<div class="col-4" id="employeetitle${emp.id}">${emp.title}</div>
                      <div class="col-4" id="employeefname${emp.id}">${emp.firstname}</div>
                      <div class="col-4" id="employeelastname${emp.id}">${emp.lastname}</div>`
            );
            btn.appendTo($("#employeeList"));
        });
    }; // end buildEmployeeList




    getAll(""); // first grab the data from the server
});

// error
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


