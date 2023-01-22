using Xunit;
using HelpdeskViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading.Tasks;

namespace CaseStudyTests
{
    public class EmployeeViewModelTests
    {
        [Fact]
        public async Task Employee_GetByEmailTest()
        {
            EmployeeViewModel vm = null;
            try
            {
                vm = new EmployeeViewModel { Email = "bs@abc.com" };
                await vm.GetByEmail();
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
            Assert.NotNull(vm.Firstname);
        }

        [Fact]
        public async Task Employee_GetByIdTest()
        {
            EmployeeViewModel vm = null;
            try
            {
                vm = new EmployeeViewModel { Id = 1 };
                await vm.GetById();
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
            Assert.NotNull(vm.Firstname);
        }

        [Fact]
        public async Task Employee_GetAllTest()
        {
            EmployeeViewModel vm = null;
            List<EmployeeViewModel> employees;
            try
            {
                employees = await vm.GetAll();
                Assert.NotNull(employees);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
        }

        [Fact]
        public async Task Employee_AddTest()
        {
            try
            {
                EmployeeViewModel newEmployee = new EmployeeViewModel
                {
                    Title = "Ms.",
                    Firstname = "Seolan",
                    Lastname = "Jin",
                    Email = "sj@abc.com",
                    Phoneno = "(555)555-5551",
                    DepartmentId = 100
                };
                await newEmployee.Add();
                Assert.True(newEmployee.Id > 0);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
        }

        [Fact]
        public async Task Employee_UpdateTest()
        {
            try
            {
                EmployeeViewModel vm = new EmployeeViewModel { Lastname = "Jin" };
                await vm.GetByEmail();
                vm.Email = vm.Email == "sj@abc.com" ? "sjs@abc.com" : "sj@abc.com";
                Assert.True(vm.Id > 0);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
        }

        [Fact]
        public async Task Employee_DeleteTest()
        {
            try
            {
                EmployeeViewModel vm = new EmployeeViewModel { Email = "sj@abc.com" };
                await vm.Delete();
                Assert.True(vm.Id > 0);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
        }

        [Fact]
        public async Task Employee_ConcurrencyTest()
        {
            int employeesUpdated = -1;

            try
            {
                EmployeeViewModel vm1 = new EmployeeViewModel { Email = "sj@abc.com" };
                EmployeeViewModel vm2 = new EmployeeViewModel { Email = "sj@abc.com" };
                await vm1.GetByEmail();
                await vm2.GetByEmail();
                vm1.Phoneno = vm1.Phoneno == "(555)555-5551" ? "(555)555-5552" : "(555)555-5551";
                if (await vm1.Update() == 1)
                {
                    vm2.Phoneno = "(666)666-6666";
                    employeesUpdated = await vm2.Update();
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error - " + ex.Message);
            }
            Assert.True(employeesUpdated == -2);
        }
    }
}
