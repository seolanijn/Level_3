using ExercisesDAL;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Threading.Tasks;

namespace ExercisesViewModels
{
    public class StudentViewModel
    {
        readonly private StudentDAO _dao;

        public string Title { get; set; }
        public string Firstname { get; set; }
        public string Lastname { get; set; }
        public string Email { get; set; }
        public string Phoneno { get; set; }
        public string Timer { get; set; }
        public int DivisionId { get; set; }
        public string DivisionName { get; set; }
        public int Id { get; set; }
        public string Picture64 { get; set; }
        
        // constructor
        public StudentViewModel()
        {
            _dao = new StudentDAO();
        }

        //
        // find student using Lastname property
        //
        public async Task GetByLastname()
        {
            try
            {
                Student stu = await _dao.GetByLastname(Lastname);
                Title = stu.Title;
                Firstname = stu.FirstName;
                Lastname = stu.LastName;
                Phoneno = stu.PhoneNo;
                Email = stu.Email;
                Id = stu.Id;
                DivisionId = stu.DivisionId;
                if (stu.Picture != null)
                {
                    Picture64 = Convert.ToBase64String(stu.Picture);
                }
                Timer = Convert.ToBase64String(stu.Timer);
            }
            catch (NullReferenceException nex)
            {
                Debug.WriteLine(nex.Message);
                Lastname = "not found";
            }
            catch(Exception ex)
            {
                Lastname = "not found";
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
        }


        //
        // find student using Id property
        //
        public async Task GetById()
        {
            try
            {
                Student stu = await _dao.GetById(Id);
                Title = stu.Title;
                Firstname = stu.FirstName;
                Lastname = stu.LastName;
                Phoneno = stu.PhoneNo;
                Email = stu.Email;
                Id = stu.Id;
                DivisionId = stu.DivisionId;
                if (stu.Picture != null)
                {
                    Picture64 = Convert.ToBase64String(stu.Picture);
                }
                Timer = Convert.ToBase64String(stu.Timer);
            }
            catch (NullReferenceException nex)
            {
                Lastname = "not found " + nex.Message;
            }
            catch (Exception ex)
            {
                Lastname = "not found";
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
        }

        //
        // Retrieve all the students
        //
        public async Task<List<StudentViewModel>> GetAll()
        {
            List<StudentViewModel> allVms = new List<StudentViewModel>();
            try
            {
                List<Student> allStudents = await _dao.GetAll();
                foreach (Student stu in allStudents)
                {
                    StudentViewModel stuVm = new StudentViewModel
                    {
                        Title = stu.Title,
                        Firstname = stu.FirstName,
                        Lastname = stu.LastName,
                        Phoneno = stu.PhoneNo,
                        Email = stu.Email,
                        Id = stu.Id,
                        DivisionId = stu.DivisionId,
                        DivisionName = stu.Division.Name,
                        Timer = Convert.ToBase64String(stu.Timer)
                    };
                    allVms.Add(stuVm);
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

        public async Task Add()
        {
            Id = -1;
            try
            {
                Student stu = new Student
                {
                    Title = Title,
                    FirstName = Firstname,
                    LastName = Lastname,
                    PhoneNo = Phoneno,
                    Email = Email,
                    DivisionId = DivisionId
                };
                Id = await _dao.Add(stu);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
        }

        public async Task<int> Update()
        {
            UpdateStatus status = UpdateStatus.Failed;
            try
            {
                Student stu = new Student
                {
                    Title = Title,
                    FirstName = Firstname,
                    LastName = Lastname,
                    PhoneNo = Phoneno,
                    Email = Email,
                    Id = Id,
                    DivisionId = DivisionId
                };
                if (Picture64 != null)
                {
                    stu.Picture = Convert.FromBase64String(Picture64);
                }

                stu.Timer = Convert.FromBase64String(Timer);
                status = await _dao.Update(stu);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return Convert.ToInt16(status);
        }

        public async Task<int> Delete()
        {
            int studentDeleted = -1;

            try
            {
                studentDeleted = await _dao.Delete(Id);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return studentDeleted;
        }
    }
}
