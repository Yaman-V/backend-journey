package com.taskmanager.api.service;

import com.taskmanager.api.model.Task;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class TaskService {
    private List<Task> tasksList = new ArrayList<>();

    public TaskService() {
        tasksList.add(new Task( "Master Spring Boot-1", "Learn annotations and MVC architecture", Task.Status.DONE));
        tasksList.add(new Task( "Master Spring Boot-2", "Learn annotations and MVC architecture", Task.Status.IN_PROGRESS));
        tasksList.add(new Task( "Review", "Review what I have learned", Task.Status.TODO));
        tasksList.add(new Task( "Submit", "Send the project to friends", Task.Status.SUSPENDED));
    }

    public List<Task> getAllTask(){
        return tasksList;
    }

    public List<Task> getTaskByTitle(String title){
        List<Task> list = new ArrayList<>();
        for(Task task : tasksList){
            if (title.equalsIgnoreCase( task.getTitle()) ){
                list.add(task);
            }
        }
        return list;
    }
    public Task addTask(Task task){
        tasksList.add(task);
        return task;
    }
    public boolean deleteTask(String title){
        return  tasksList.removeIf(task -> title.equalsIgnoreCase(task.getTitle()));
    }
}
