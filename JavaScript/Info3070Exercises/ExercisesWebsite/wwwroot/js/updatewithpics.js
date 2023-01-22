$(function () {

    $("#getbutton").click(async (e) => {
        try {
            let lastname = $("#TextBoxFindLastname").val();
            $("#status").text("Please wait...");
            $("#myModal").modal("toggle");
            let response = await fetch(`/api/student/${lastname}`);
            if (response.ok)
            {
                let data = await response.json();
                if (data.lastname !== "not found") {
                    $("#TextBoxEmail").val(data.email);
                    $("#TextBoxTitle").val(data.title);
                    $("#TextBoxFirstname").val(data.firstname);
                    $("#TextBoxLastname").val(data.lastname);
                    $("#TextBoxPhone").val(data.phoneno);
                    $("#status").text("student found");
                    $("#ImageHolder").html(`<img height="120" width="110" src="data:img/png;base64,${data.picture64}"/>`);

                    sessionStorage.setItem("id", data.id);
                    sessionStorage.setItem("divisionId", data.divisionId);
                    sessionStorage.setItem("timer", data.timer);
                    sessionStorage.setItem("picture", data.picture64);

                } else {
                    $("#TextBoxFirstname").val("not found");
                    $("#TextBoxLastname").val("");
                    $("#TextBoxEmail").val("");
                    $("#TextBoxTitle").val("");
                    $("#TextBoxPhone").val("");
                    $("#status").text("no such student");
                }
            } else if (response.status !== 404) {
                let problemJson = await response.json();
                errorRtn(problemJson, response.status);
            } else {
                $("#status").text("no such path on server");
            }
        } catch (error) {
            $("#status").text(error.message);
        }

    });


    $("#updatebutton").click(async (e) => {
        try {

            stu = new Object();

            stu.title = $("#TextBoxTitle").val();
            stu.firstname = $("#TextBoxFirstname").val();
            stu.lastname = $("#TextBoxLastname").val();
            stu.phoneno = $("#TextBoxPhone").val();
            stu.email = $("#TextBoxEmail").val();
            stu.divisionName = "";
            stu.picture64 = "";

            stu.id = parseInt(sessionStorage.getItem("id"));
            stu.divisionId = parseInt(sessionStorage.getItem("divisionId"));
            stu.timer = sessionStorage.getItem("timer");
            sessionStorage.getItem("picture")
                ? stu.picture64 = sessionStorage.getItem("picture")
                : stu.picture64 = null;

            let response = await fetch("api/student", {
                method: "PUT",
                headers: { "Content-Type": "application/json; charset=utf-8" },
                body: JSON.stringify(stu)
            });

            if (response.ok) {
                let payload = await response.json();
                $("#status").text(payload.msg);
            } else if (response.status !== 404) {
                let problemJson = await response.json();
                errorRtn(problemJson, response.status);
            } else {
                $("#status").text("no such path on server");
            }
        } catch (error) {
            $("#status").text(error.message);
            console.table(error);
        }
        $("#myModal").modal("toggle");
    });

    $("input:file").change(() => {
        const reader = new FileReader();
        const file = $("#uploader")[0].files[0];

        file ? reader.readAsBinaryString(file) : null;

        reader.onload = (readerEvt) => {
            const binaryString = reader.result;
            const encodedString = btoa(binaryString);
            sessionStorage.setItem('picture', encodedString);
        };
    });
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







