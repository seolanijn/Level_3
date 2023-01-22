using System;
using Xunit;
using ExercisesDAL;
using System.Linq;
using System.Diagnostics;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace ExerciseTests
{
    public class DAOTests
    {
        [Fact]
        public async Task Student_GetByLastnameTest()
        {
            StudentDAO dao = new StudentDAO();
            Student selectedStudent = await dao.GetByLastname("Apple");
            Assert.NotNull(selectedStudent);
        }
        [Fact]
        public async Task Student_GetByIdTest()
        {
            StudentDAO dao = new StudentDAO();
            Student selectedStudent = await dao.GetById(1003);
            Assert.NotNull(selectedStudent);
        }
        [Fact]
        public async Task Student_GetAllTest()
        {
            StudentDAO dao = new StudentDAO();
            List<Student> selectedStudent = await dao.GetAll();
            Assert.NotNull(selectedStudent);
        }
        [Fact]
        public async Task Student_AddTest()
        {
            StudentDAO dao = new StudentDAO();
            Student newStudent = new Student
            {
                FirstName = "Seolan",
                LastName = "Jin",
                PhoneNo = "(555) 555-1234",
                Title = "Ms.",
                DivisionId = 10,
                Email = "en@someschool.com"
            };
            await dao.Add(newStudent);
            Assert.NotNull(newStudent);
        }

        [Fact]
        public async Task Student_UpdateTest()
        {
            StudentDAO dao = new StudentDAO();
            Student selectedStudent = await dao.GetByLastname("Jin");
            if (selectedStudent != null)
            {
                string oldEmail = selectedStudent.Email;
                string newEmail = oldEmail == "en@someschool.com" ? "enn@someschool.com" : "en@someschool.com";
                selectedStudent.Email = newEmail;
            }
            Assert.True(await dao.Update(selectedStudent) == UpdateStatus.Ok);
        }
        [Fact]
        public async Task Student_DeleteTest()
        {
            StudentDAO dao = new StudentDAO();
            await dao.Delete(1004);
        }
        [Fact]
        public async Task Student_ConcurrencyTest()
        {
            StudentDAO dao1 = new StudentDAO();
            StudentDAO dao2 = new StudentDAO();
            Student studentForUpdate1 = await dao1.GetByLastname("Jin");
            Student studentForUpdate2 = await dao2.GetByLastname("Jin");

            if (studentForUpdate1 != null)
            {
                string oldPhoneNo = studentForUpdate1.PhoneNo;
                string newPhoneNo = oldPhoneNo == "519-555-1234" ? "555-555-5555" : "519-555-1234";
                studentForUpdate1.PhoneNo = newPhoneNo;
                if (await dao1.Update(studentForUpdate1) == UpdateStatus.Ok)
                {
                    studentForUpdate2.PhoneNo = "666-666-6666";
                    Assert.True(await dao2.Update(studentForUpdate2) == UpdateStatus.Stale);
                }
                else
                    Assert.True(false);
            }
        }

        [Fact]
        public async Task Student_LoadPicsTest()
        {
            try
            {
                DALUtil util = new DALUtil();
                Assert.True(await util.AddStudentPicsToDb());
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
        }
    }
}
