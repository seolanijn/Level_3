using System;
using Xunit;
using ExercisesDAL;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;

namespace ExerciseTests
{
    public class LinqTests
    {
        [Fact]
        public void Test1()
        {
            SomeSchoolContext _db = new SomeSchoolContext();
            var selectedStudent = from stu in _db.Students
                                    where stu.Id == 2
                                    select stu;

            Assert.True(selectedStudent.Count() > 0);
        }

        [Fact]
        public void Test2()
        {
            SomeSchoolContext _db = new SomeSchoolContext();
            var selectedStudent = from stu in _db.Students
                                    where stu.Title == "Ms." || stu.Title == "Mrs."
                                    select stu;

            Assert.True(selectedStudent.Count() > 0);
        }

        [Fact]
        public void Test3()
        {
            SomeSchoolContext _db = new SomeSchoolContext();
            var selectedStudent = from stu in _db.Students
                                    where stu.Division.Name == "Design"
                                    select stu;
            Assert.True(selectedStudent.Count() > 0);
        }

        [Fact]
        public void Test4()
        {
            SomeSchoolContext _db = new SomeSchoolContext();
            var selectedStudent = _db.Students.FirstOrDefault(stu => stu.Id == 2);
            Assert.True(selectedStudent.FirstName == "Teachers");
        }

        [Fact]
        public void Test5()
        {
            SomeSchoolContext _db = new SomeSchoolContext();
            var selectedStudent = _db.Students.Where(stu => stu.Title == "Ms." || stu.Title == "Mrs.");
            Assert.True(selectedStudent.Count() > 0);
        }

        [Fact]
        public void Test6()
        {
            SomeSchoolContext _db = new SomeSchoolContext();
            var selectedStudent = _db.Students.Where(stu => stu.Division.Name == "Design");
            Assert.True(selectedStudent.Count() > 0);
        }

        [Fact]
        public async Task Test7()
        {
            SomeSchoolContext _db = new SomeSchoolContext();
            Student selectedStudent = await _db.Students.FirstOrDefaultAsync(stu => stu.Id == 1002);
            if (selectedStudent != null)
            {
                string oldEmail = selectedStudent.Email;
                string newEmail = oldEmail == "js@someschool.com" ? "sj@someschool.com" : "js@someschool.com";
                selectedStudent.Email = newEmail;
                _db.Entry(selectedStudent).CurrentValues.SetValues(selectedStudent);
            }
            Assert.True(await _db.SaveChangesAsync() == 1); //rows updated
        }

        [Fact]
        public async Task Test8()
        {
            SomeSchoolContext _db = new SomeSchoolContext();
            Student newStudent = new Student
            {
                FirstName = "Seolan",
                LastName = "Jin",
                PhoneNo = "(555)555-1234",
                Title = "Mr.",
                DivisionId = 10,
                Email = "js@someschool.com"
            };
            await _db.Students.AddAsync(newStudent);
            await _db.SaveChangesAsync();
            Assert.True(newStudent.Id > 0); //should be populated after save
        }

        [Fact]
        public async Task Test9()
        {
            SomeSchoolContext _db = new SomeSchoolContext();
            Student selectedStudent = await _db.Students.FirstOrDefaultAsync(stu => stu.LastName== "Jin");
            if (selectedStudent != null)
            {
                _db.Students.Remove(selectedStudent);
                Assert.True(await _db.SaveChangesAsync() == 1); //# of rows deleted
            }
            else
            {
                Assert.True(false);
            }
        }
    }
}
