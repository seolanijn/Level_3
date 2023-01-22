/* name: EmloyeeViewModel.cs
 * description: it will provide methods and state for a GUI application
 */
using HelpdeskDAL;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Threading.Tasks;

namespace HelpdeskViewModels
{
    public class EmployeeViewModel
    {
        private readonly EmployeeDAO _dao;

        public string Title { get; set; }
        public string Firstname { get; set; }
        public string Lastname { get; set; }
        public string Email { get; set; }
        public string Phoneno { get; set; }
        public string Timer { get; set; }
        public int DepartmentId { get; set; }
        public string DepartmentName { get; set; }
        public int Id { get; set; }
        public bool IsTech { get; set; }
        public string StaffPicture64 { get; set; }

        // constructor
        public EmployeeViewModel()
        {
            _dao = new EmployeeDAO();
        }

        // find employee by email
        public async Task GetByEmail()
        {
            try
            {
                Employee emp = await _dao.GetByEmail(Email);
                Title = emp.Title;
                Firstname = emp.FirstName;
                Lastname = emp.LastName;
                Phoneno = emp.PhoneNo;
                Email = emp.Email;
                Id = emp.Id;
                DepartmentId = emp.DepartmentId;
                //DepartmentName = emp.Department.DepartmentName;
                if (emp.StaffPicture != null)
                {
                    StaffPicture64 = Convert.ToBase64String(emp.StaffPicture);
                }
                Timer = Convert.ToBase64String(emp.Timer);
            }
            catch (NullReferenceException nex)
            {
                Debug.WriteLine(nex.Message);
                Email = "not found";
            }
            catch (Exception ex)
            {
                Email = "not found";
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
        }

        // find employee by id
        public async Task GetById()
        {
            try
            {
                Employee emp = await _dao.GetById(Id);
                Title = emp.Title;
                Firstname = emp.FirstName;
                Lastname = emp.LastName;
                Phoneno = emp.PhoneNo;
                Email = emp.Email;
                Id = emp.Id;
                DepartmentId = emp.DepartmentId;
                //DepartmentName = emp.Department.DepartmentName;
                if (emp.StaffPicture != null)
                {
                    StaffPicture64 = Convert.ToBase64String(emp.StaffPicture);
                }
                Timer = Convert.ToBase64String(emp.Timer);
            }
            catch (NullReferenceException nex)
            {
                Email = "not found" + nex.Message;
            }
            catch (Exception ex)
            {
                Email = "not found";
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                  MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
        }

        // find all employees
        public async Task<List<EmployeeViewModel>> GetAll()
        {
            List<EmployeeViewModel> allVms = new List<EmployeeViewModel>();
            try
            {
                List<Employee> allemployees = await _dao.GetAll();
                foreach (Employee emp in allemployees)
                {
                    EmployeeViewModel empVm = new EmployeeViewModel
                    {
                        Title = emp.Title,
                        Firstname = emp.FirstName,
                        Lastname = emp.LastName,
                        Phoneno = emp.PhoneNo,
                        Email = emp.Email,
                        Id = emp.Id,
                        DepartmentId = emp.DepartmentId,
                       // DepartmentName = emp.Department.DepartmentName,
                        Timer = Convert.ToBase64String(emp.Timer)
                    };
                    if (StaffPicture64 != null)
                    {
                        emp.StaffPicture = Convert.FromBase64String(StaffPicture64);
                    }
                    allVms.Add(empVm);
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return allVms;
        }

        // add employee
        public async Task Add()
        {
            Id = -1;
            try
            {
                Employee emp = new Employee
                {
                    Title = Title,
                    FirstName = Firstname,
                    LastName = Lastname,
                    PhoneNo = Phoneno,
                    Email = Email,
                    DepartmentId = DepartmentId
                };
                Id = await _dao.Add(emp);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                  MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
        }

        // update employee
        public async Task<int> Update()
        {
            UpdateStatus status = UpdateStatus.Failed;
            try
            {
                Employee emp = new Employee
                {
                    Title = Title,
                    FirstName = Firstname,
                    LastName = Lastname,
                    PhoneNo = Phoneno,
                    Email = Email,
                    Id = Id,
                    DepartmentId = DepartmentId
                };
                if (StaffPicture64 != null)
                {
                    emp.StaffPicture = Convert.FromBase64String(StaffPicture64);
                }

                emp.Timer = Convert.FromBase64String(Timer);
                status = await _dao.Update(emp);
                //employeeUpdated = await _dao.Update(emp);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return Convert.ToInt16(status);
        }

        // delete emloyee
        public async Task<int> Delete()
        {
            int employeeUpdated = -1;

            try
            {
                employeeUpdated = await _dao.Delete(Id);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return employeeUpdated;
        }


    }
}
