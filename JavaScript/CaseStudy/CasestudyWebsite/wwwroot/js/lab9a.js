$(function () {

    $("#getbutton").click(async (e) => {   
        try {
            let lastname = $("#TextBoxFindLastname").val();
            $("#status").text("Please wait...");
            $("#myModal").modal("toggle");
            let response = await fetch(`/api/employee/${lastname}`);
            if (response.ok)
            {
                let data = await response.json();   // this returns a promise, so we await it
                if (data.lastname !== "not found") {
                    $("#TextBoxEmail").text(data.email);
                    $("#TextBoxTitle").text(data.title);
                    $("#TextBoxFirstname").text(data.firstname);
                    $("#TextBoxLastname").text(data.lastname);
                    $("#TextBoxPhone").text(data.phoneno);
                    $("#status").text("employee found");

                    sessionStorage.setItem("id", data.id);
                    sessionStorage.setItem("divisionId", data.divisionId);
                    sessionStorage.setItem("timer", data.timer);
                } else {
                    $("#TextBoxFirstname").text("not found");
                    $("#TextBoxLastname").text("");
                    $("#TextBoxEmail").text("");
                    $("#TextBoxTitle").text("");
                    $("#TextBoxPhone").text("");
                    $("#status").text("no such employee");
                }
            } else if (response.status !== 404) { // probably some other client side error
                let problemJson = await response.json();
                errorRtn(problemJson, response.status);
            } else {
                $("#status").text("no such path on server");
            }
        } catch (error) { // catastrophic
            $("#status").text(error.message);
        }
    }); // click event



    $("#Updatebutton").click(async (e) => {
        try {
            stu = new Object();
            stu.title = $("#TextBoxTitle").val();
            stu.firstname = $("#TextBoxFirstname").val();
            stu.lastname = $("#TextBoxLastName").val();
            stu.phoneno = $("#TextBoxPhone").val();
            stu.email = $("#TextBoxEmail").val();
            stu.divisionName = "";
            stu.picture64 = "";

            stu.id = parseInt(sessionStorage.getItem("id"));
            stu.divisionId = parseInt(sessionStorage.getItem("divisionId"));
            stu.timer = sessionStorage.getItem("timer");

            let response = await fetch("api/employee", {
                method: "PUT",
                headers: { "Content-Type": "application/json; charset-utf-8" },
                body: JSON.stringify(stu)
            });


            if (response.ok) {
                let payload = await response.json();
                $("#status").text(payload.msg);
            } else if (response.status !== 404) { // probably some other client side error
                let problemJson = await response.json();
                errorRtn(problemJson, response.status);
            } else {
                $("#status").text("no such path on server");
            }
        } catch (error) { // catastrophic
            $("#status").text(error.message);
            console.table(error);
        }
    }); // click event

}); // jQuery ready method

// server was reached but server had a problem with the call
const errorRtn = (problemJson, status) => {
    if (status > 499) {
        $("#status").text("Problem server side, see debug console");
    }
    else {
        let keys = Objects.keys(problemJson.errors)
        problem = {
            status: status,
            statusText: problemJson.errors[keys[0]][0], // first error
        };
        $("#status").text("Problem client side, see browser console");
        console.log(problem);
    }
}







