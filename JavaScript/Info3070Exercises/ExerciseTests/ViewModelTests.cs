using Xunit;
using ExercisesViewModels;
using System.Threading.Tasks;
using System;
using System.Diagnostics;
using System.Collections.Generic;


namespace ExerciseTests
{
    public class ViewModelTests
    {
        [Fact]
        public async Task Student_GetByLastnameTest()
        {
            StudentViewModel vm = null;
            try
            {
                vm = new StudentViewModel { Lastname = "Nose" };
                await vm.GetByLastname();
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
            Assert.NotNull(vm.Firstname);
        }

        [Fact]
        public async Task Student_GetByIdTest()
        {
            StudentViewModel vm = null;
            try
            {
                vm = new StudentViewModel { Id = 1 };
                await vm.GetById();
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
            Assert.NotNull(vm.Firstname);
        }

        [Fact]
        public async Task Student_GetAllTest()
        {
            StudentViewModel vm = null;
            List<StudentViewModel> students;
            try
            {
                students = await vm.GetAll();
                Assert.NotNull(students);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
        }

        [Fact]
        public async Task Student_AddTest()
        {
            try
            {
                StudentViewModel newStudent = new StudentViewModel
                {
                    Title = "Ms.",
                    Firstname = "Emily",
                    Lastname = "Apple",
                    Email = "ep@abc.com",
                    Phoneno = "(555)555-5551",
                    DivisionId = 10
                };
                await newStudent.Add();
                Assert.True(newStudent.Id > 0);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
        }

        [Fact]
        public async Task Student_UpdateTest()
        {
            try
            {
                StudentViewModel vm = new StudentViewModel { Lastname = "Apple" };
                await vm.GetByLastname();
                vm.Email = vm.Email == "sj@abc.com" ? "sjs@abc.com" : "sj@abc.com";
                Assert.True(vm.Id > 0);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
        }

        [Fact]
        public async Task Student_DeleteTest()
        {
            try
            {
                StudentViewModel vm = new StudentViewModel { Lastname = "Apple" };
                await vm.Delete();
                Assert.True(vm.Id > 0);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
        }

        [Fact]
        public async Task Student_ConcurrencyTest()
        {
            int studentsUpdated = -1;

            try
            {
                StudentViewModel vm1 = new StudentViewModel { Lastname = "Jin" };
                StudentViewModel vm2 = new StudentViewModel { Lastname = "Jin" };
                await vm1.GetByLastname();
                await vm2.GetByLastname();
                vm1.Phoneno = vm1.Phoneno == "(555)555-5551" ? "(555)555-5552" : "(555)555-5551";
                if (await vm1.Update() == 1)
                {
                    vm2.Phoneno = "(666)666-6666";
                    studentsUpdated = await vm2.Update();
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
            Assert.True(studentsUpdated == -2);
        }
    }
}
