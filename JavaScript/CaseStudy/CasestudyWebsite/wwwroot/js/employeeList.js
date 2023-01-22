$(function () {

    const getAll = async (msg) => {
        try {
            $("#employeeList").text("Finding Employee Information...");
            let response = await fetch(`/api/employee`);
            if (response.ok) {

                $("#employeeList").text("Finding Employee Information...2");
                let payload = await response.json();
                buildEmployeeList(payload);
                msg === "" ?
                    $("#status").text("Employees Loaded") : $("#status").text(`${msg} -Employees Loaded`);
            } else if (response.status !== 404) { // probably some other client side error
                let problemJson = await response.json();
                errorRtn(problemJson, response.status);
            } else {
                $("#status").text("no such path on server");
            }
        } catch (error) { // catastrophic
            $("#status").text(error.message);
        }
    };

    const clearModalFields = () => {
        $("#TextBoxTitle").val("");
        $("#TextBoxFirstname").val("");
        $("#TextBoxLastName").val("");
        $("#TextBoxPhone").val("");
        $("#TextBoxEmail").val("");
        sessionStorage.removeItem("Id");
        sessionStorage.removeItem("divisionId");
        sessionStorage.removeItem("timer");
    }

    $("#employeeList").click((e) => {
        clearModalFields();
        if (!e) e = window.event;
        let id = e.target.parentNode.id;
        if (id === "employeeList" || id === "") {
            id = e.target.id;
        }
        if (id !== "status" && id !== "heading") {
            let data = JSON.parse(sessionStorage.getItem("allemployees"));
            data.map(employee => {
                if (employee.id === parseInt(id)) {
                    $("#TextBoxTitle").val(employee.title);
                    $("#TextBoxFirstname").val(employee.firstname);
                    $("#TextBoxLastname").val(employee.lastname);
                    $("#TextBoxPhone").val(employee.phoneno);
                    $("#TextBoxEmail").val(employee.email);
                    sessionStorage.setItem("id", employee.id);
                    sessionStorage.setItem("divisionId", employee.divisionId);
                    sessionStorage.setItem("timer", employee.timer);
                    $("#modalstatus").text("update data");
                    $("#myModal").modal("toggle");
                }
            });
        } else {
            return false;
        }
    });

    $("#updatebutton").click(async (e) => {
        try {
            //
            emp = new Object();
            //
            emp.title = $("#TextBoxTitle").val();
            emp.firstname = $("#TextBoxFirstname").val();
            emp.lastname = $("#TextBoxLastname").val();
            emp.phoneno = $("#TextBoxPhone").val();
            emp.email = $("#TextBoxEmail").val();
            //
            emp.id = parseInt(sessionStorage.getItem("id"));
            emp.divisionId = parseInt(sessionStorage.getItem("divisionId"));
            emp.timer = sessionStorage.getItem("timer");
            emp.picture64 = null;

            //
            let response = await fetch("api/employee", {
                method: "PUT",
                headers: { "Content-Type": "application/json; charset=utf-8" },
                body: JSON.stringify(emp)
            });

            if (response.ok)
            {
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
    });

    const buildEmployeeList = (data) => {
        $("#employeeList").empty();
        div = $(`<div class="list-group-item text-white bg-secondary row d-flex" id="status">Employee Info</div>
                 <div class= "list-group-item row d-flex text-center" id="heading">
                 <div class="col-4 h4">Title</div>
                 <div class="col-4 h4">First</div>
                 <div class="col-4 h4">Last</div>
                 </div>`);
        div.appendTo($("#employeeList"));
        sessionStorage.setItem("allemployees", JSON.stringify(data));
        data.map(emp => {
            btn = $(`<button class="list-group-item row d-flex" id="${emp.id}">`);
            btn.html(`<div class="col-4" id="employeetitle${emp.id}">${emp.title}</div>
                      <div class="col-4" id="employeefname${emp.id}">${emp.firstname}</div>
                      <div class="col-4" id="employeelastname${emp.id}">${emp.lastname}</div>`
            );
            btn.appendTo($("#employeeList"));
        });
    }

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
